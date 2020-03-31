#pragma once

#include <memory>
#include <string>

#include "frame_listener.h"

#ifndef LIBFREENECT2_VERSION
#define LIBFREENECT2_VERSION "simulator version"
#endif

namespace libfreenect2
{


class Freenect2Device
{
public:
    std::string getFirmwareVersion() { return "simulator version"; }
    std::string getSerialNumber() { return "sim"; }

    void setColorFrameListener(FrameListener* listener) {}
    void setIrAndDepthFrameListener(FrameListener* listener) {}
    bool start() { return true; }
    void stop() {}
    void close() {}
};

class Freenect2
{
    std::shared_ptr<Freenect2Device> device;

public:
    std::string getDefaultDeviceSerialNumber() { return "sim"; }
    Freenect2Device* openDevice(std::string serial)
    {
        device = std::make_shared<Freenect2Device>();
    }
    int enumerateDevices() { return 1; }
};


}  // namespace libfreenect2

