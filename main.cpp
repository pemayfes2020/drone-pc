#include "cmdline.h"
#include "drone.hpp"
#include "esp32.hpp"
#include "kinect.hpp"
#include "localization.hpp"
#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"

#include <opencv/cv.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    cmdline::parser parser;
    parser.add<std::string>("btport", 'b', "esp32 port", false, "/dev/rfcomm0");
    parser.add<std::string>("sock-ar", 'a', "ar unix domain socket", false, "/tmp/ar.sock");
    parser.add<std::string>("sock-viz", 'v', "visualizer unix domain socket", false, "/tmp/viz.sock");

    parser.parse_check(argc, argv);

    ThreadRoom::setExitHandler();

    ESP32::start(parser.get<std::string>("btport"));

    Drone::start(parser.get<std::string>("sock-ar"));

    //Kinect::start();

    UnixSocket::Client visualizer(parser.get<std::string>("sock-viz"));

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

