#include "control/localization.hpp"
#include "esp32/esp32.hpp"
#include "kinect/kinect.hpp"

#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"
#include "unit.hpp"

#include <cmdline.h>
#include <opencv/cv.hpp>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

Time elapsed_time(const std::chrono::system_clock::time_point& start)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() * 1.0 * 1.0_us;
}

std::string getDefaultLogFileName()
{
    std::time_t now = std::time(nullptr);
    std::tm* local_now = std::localtime(&now);
    std::stringstream ss;
    ss << std::setw(4) << local_now->tm_year + 1900
       << std::setw(2) << local_now->tm_mon + 1
       << std::setw(2) << local_now->tm_mday + 1
       << std::setw(2) << local_now->tm_hour
       << std::setw(2) << local_now->tm_min + 1
       << std::setw(2) << local_now->tm_sec;

    return ss.str();
}

class VisualizerWrapper
{
    bool visualize;
    std::unique_ptr<UnixSocket::Client> socket_client;
    std::ofstream out;
    std::unique_ptr<Logger::Logger> logger;

public:
    VisualizerWrapper(bool visualize, const std::string socket, const std::string log_file)
        : visualize{visualize},
          socket_client{visualize ? std::make_unique<UnixSocket::Client>(socket) : nullptr},
          out{log_file},
          logger{std::make_unique<Logger::Logger>(Logger::Logger::Data, out)}
    {
    }

    VisualizerWrapper& operator<<(const Common::Visualizer::VectorData& data)
    {
        if (visualize) {
            socket_client->write(std::as_const(data));
        }
        logger->write(data);
    }
};

int main(int argc, char** argv)
{
    cmdline::parser parser;
    parser.add<std::string>("btport", 'b', "esp32 port", false, "/dev/rfcomm0");
    parser.add<std::string>("sock-ar", 'a', "ar unix domain socket", false, "/tmp/ar.sock");
    parser.add<std::string>("sock-viz", 'v', "visualizer unix domain socket", false, "/tmp/viz.sock");
    parser.add<std::string>("log-file", 'f', "log output file", false, getDefaultLogFileName());
    parser.add("no-visualizer", 'n', "launch without visualizer");

    parser.parse_check(argc, argv);

    ThreadRoom::setExitHandler();

    //ESP32::start(parser.get<std::string>("btport"));

    //Kinect::start();

    UnixSocket::Client drone(parser.get<std::string>("sock-ar"));

    VisualizerWrapper visualizer{
        parser.exist("no-visualizer"),
        parser.get<std::string>("sock-viz"),
        parser.get<std::string>("log-file")};

    Vel zero_vel = 0.0_mm / 1.0_s;
    drone.write<Common::Drone::SendData>({zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::TAKEOFF});

    auto start_time = std::chrono::system_clock::now();

    while (true) {
        //auto [rgb, ir, depth] = Kinect::getImages();

        auto [roll, pitch, yaw, z, vx, vy, vz] = drone.read<Common::Drone::ReceiveData>().attachUnit();
        std::cout << roll << ' ' << pitch << ' ' << yaw << std::endl;

        visualizer << Common::Visualizer::VectorData(elapsed_time(start_time), 0.0_mm, 0.0_mm, z, roll, pitch, yaw);

        drone.write(Common::Drone::SendData{zero_vel, zero_vel, zero_vel, zero_vel, Common::Drone::Command::VELOCITY});
    }

    return 0;
}
