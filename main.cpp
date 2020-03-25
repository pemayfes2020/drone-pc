#include "drone.hpp"
#include "esp32.hpp"
#include "kinect.hpp"
#include "localization.hpp"
#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"

#include <opencv/cv.hpp>

#include <iostream>

int main()
{
    ThreadRoom::setExitHandler();

    ESP32::start("/dev/rfcomm0");

    Drone::start("/tmp/ar.sock");

    //Kinect::start();

    UnixSocket::Client visualizer("/tmp/viz.sock");

    Vel zero_vel = 0.0_mm / 1.0_s;
    Drone::send(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::TAKEOFF});

    visualizer.write<Common::Visualizer::VectorData>(
        Common::Visualizer::VectorData{0.0_mm, 0.0_mm, 0.0_mm, 0.0_rad, 0.0_rad, 0.0_rad});


    while (true) {
        //auto [rgb, ir, depth] = Kinect::getImages();

        auto [roll, pitch, yaw, z, vx, vy, vz] = Drone::read().attachUnit();
        std::cout << roll << ' ' << pitch << ' ' << yaw << std::endl;

        visualizer.write<Common::Visualizer::VectorData>(
            Common::Visualizer::VectorData{0.0_mm, 0.0_mm, 0.0_mm, roll, pitch, yaw});

        Drone::send(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::VELOCITY});
    }

    return 0;
}
