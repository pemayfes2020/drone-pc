#include "drone.hpp"
#include "esp32.hpp"
#include "kinect.hpp"
#include "localization.hpp"
#include "safe_exit.hpp"

#include <opencv/cv.hpp>

#include <iostream>

int main()
{
    ThreadRoom::setExitHandler();

    ESP32::start("/dev/rfcomm0");

    Drone::start();

    Kinect::start();

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();
    }

    return 0;
}
