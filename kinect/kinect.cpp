#include "kinect.hpp"
#include "logger.hpp"
#include "safe_exit.hpp"

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Kinect
{

// 実装は https://openkinect.github.io/libfreenect2/ に基づく

class KinectManager
{
private:
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device* device;
    std::shared_ptr<libfreenect2::SyncMultiFrameListener> listener;
    libfreenect2::FrameMap frames;

    Images images;

public:
    KinectManager()
        : freenect2{}, device{nullptr},
          listener{}, frames{}
    {
        // Logging
        libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Debug));

        FileLogger* filelogger = new FileLogger(getenv("LOGFILE"));
        if (filelogger->good()) {
            libfreenect2::setGlobalLogger(filelogger);
        } else {
            delete filelogger;
        }

        // Discover Devices
        if (freenect2.enumerateDevices() == 0) {
            std::cerr << "no device connected!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::string serial = freenect2.getDefaultDeviceSerialNumber();

        // Open and Configure the Device
        device = freenect2.openDevice(serial);

        listener = std::make_shared<libfreenect2::SyncMultiFrameListener>(
            libfreenect2::Frame::Color
            | libfreenect2::Frame::Ir
            | libfreenect2::Frame::Depth);

        device->setColorFrameListener(listener.get());
        device->setIrAndDepthFrameListener(listener.get());

        // Start the Device
        if (!device->start()) {
            std::cerr << "Cannot Open Device" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Output Informations
        std::cout << "Version: " << LIBFREENECT2_VERSION << std::endl;

        std::cout << "[info] [Kinect] device serial: " << device->getSerialNumber() << std::endl;
        std::cout << "[info] [Kinect] device firmware: " << device->getFirmwareVersion() << std::endl;
    }
    ~KinectManager()
    {
        device->stop();
        device->close();
    }

    void update()
    {
        if (!listener->waitForNewFrame(frames, 10 * 1000)) {
            std::cerr << "[error] [Kinect] timeout!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];

        cv::flip(cv::Mat{
                     (int)rgb->height, (int)rgb->width,
                     CV_8UC4, rgb->data},
            images.rgb, 1);

        cv::flip(cv::Mat{
                     (int)ir->height, (int)ir->width,
                     CV_32F, ir->data},
            images.ir, 1);

        cv::flip(cv::Mat{
                     (int)depth->height, (int)depth->width,
                     CV_32F, depth->data},
            images.depth, 1);

        listener->release(frames);
    }

    Images getImages()
    {
        return images;
    }
};

Images images;
std::mutex mutex_images;
std::atomic_bool image_ready = false;

void start()
{
    std::thread{
        [&images, &image_ready]() mutable {
            ThreadRoom::enter();

            try {
                KinectManager kinect;

                while (!ThreadRoom::toExit()) {
                    kinect.update();
                    std::lock_guard<std::mutex> lock{mutex_images};
                    images = kinect.getImages();
                    image_ready = true;
                }

            } catch (ThreadRoom::thread_abort) {
                ThreadRoom::exit();
                return;
            }
        }}
        .detach();
}

Images getImages()
{
    while (!image_ready) {
    }
    std::lock_guard<std::mutex> lock{mutex_images};
    return images;
}

}  // namespace Kinect
