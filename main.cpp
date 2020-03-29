#include "control/localization.hpp"
#include "esp32/esp32.hpp"
#include "kinect/kinect.hpp"

#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"

#include <cmdline.h>
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

    //ESP32::start(parser.get<std::string>("btport"));

    //Kinect::start();

    UnixSocket::Client drone(parser.get<std::string>("sock-ar"));

    UnixSocket::Client visualizer(parser.get<std::string>("sock-viz"));

    Vel zero_vel = 0.0_mm / 1.0_s;
    drone.write<Common::Drone::SendData>({zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::TAKEOFF});

    visualizer.write<Common::Visualizer::VectorData>(
        Common::Visualizer::VectorData{0.0_mm, 0.0_mm, 0.0_mm, 0.0_rad, 0.0_rad, 0.0_rad});


    while (true) {
        //auto [rgb, ir, depth] = Kinect::getImages();

        auto [roll, pitch, yaw, z, vx, vy, vz] = drone.read<Common::Drone::ReceiveData>().attachUnit();
        std::cout << roll << ' ' << pitch << ' ' << yaw << std::endl;

        visualizer.write<Common::Visualizer::VectorData>(
            Common::Visualizer::VectorData{0.0_mm, 0.0_mm, z, roll, pitch, yaw});

        drone.write<Common::Drone::SendData>(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::VELOCITY});
    }

    return 0;
}

