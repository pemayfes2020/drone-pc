#include "drone.hpp"
#include "esp32.hpp"
#include "kinect.hpp"
#include "localization.hpp"
#include "message_types.hpp"
#include "safe_exit.hpp"

#include <opencv/cv.hpp>

#include <iostream>

int main()
{
    ThreadRoom::setExitHandler();

    ESP32::start("/dev/rfcomm0");

    Drone::start("/tmp/ar.sock");

    Kinect::start();

    Vel zero_vel = 0.0_mm / 1.0_s;
    Drone::send(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::TAKEOFF});

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();

        auto [roll, pitch, yaw, z, vx, vy, vz] = Drone::read().attachUnit();
        std::cout << roll << ' ' << pitch << ' ' << yaw << std::endl;

        Drone::send(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::VELOCITY});
    }

    return 0;
}
