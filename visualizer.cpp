#include "graphic/graphic.hpp"
#include "message_types_viz.hpp"
#include "socket.hpp"
#include "visual/drone.hpp"
#include "visual/project_room.hpp"

#include <Eigen/Core>
#include <cmdline.h>

#include <atomic>
#include <cmath>
#include <csignal>
#include <string>
#include <thread>

Eigen::Vector3d pos{0.0, 0.0, 0.0};
Eigen::Vector3d rot{0.0, 0.0, 0.0};

void callback()
{
    Common::Visual::Drone::update(pos, rot);
}

std::atomic_bool exit_flag = false;
void sigint_handler(int)
{
    exit_flag = true;
}

int main(int argc, char** argv)
{
    cmdline::parser parser;
    parser.add<std::string>("socket", 's', "unix domain socket", false, "/tmp/viz.sock");
    parser.add<std::string>("model", 'm', "Drone STL Model File", false, "../resource/ardrone.stl");

    signal(SIGINT, sigint_handler);

    UnixSocket::Server server(parser.get<std::string>("socket"));

    std::thread comm_thread{
        [&exit_flag, &pos, &rot](UnixSocket::Server& server) {
            while (!exit_flag) {
                auto data = server.read<Common::Visualizer::VectorData>();
                pos << data.x, data.y, data.z;
                rot << data.roll, data.pitch, data.yaw;
            }
        },
        std::ref(server)};

    Graphic::init(argc, argv);

    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    Common::Visual::ProjectRoom::construct();
    Common::Visual::Drone::construct(parser.get<std::string>("model"));

    Graphic::start(callback);

    comm_thread.join();

    return 0;
}
