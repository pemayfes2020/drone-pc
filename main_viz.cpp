#include "cmdline.h"
#include "graphic.hpp"
#include "message_types_viz.hpp"
#include "socket.hpp"

#include <Eigen/Core>

#include <atomic>
#include <cmath>
#include <csignal>
#include <string>
#include <thread>

Eigen::Vector3f pos;
Eigen::Vector3f rot;

Graphic::Object drone;
Graphic::Object sphere;

void callback(std::vector<Graphic::Object>& objs)
{
    drone.shape->position = pos;
    drone.shape->rotation = rot;
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

    pos << 0.0f, 0.0f, 0.0f;
    rot << 0.0f, 0.0f, 0.0f;

    drone = Graphic::addSTLModel(pos, rot, "../resource/ardrone.stl", Color{1.0, 1.0, 0.0});

    Graphic::start(callback);

    comm_thread.join();

    return 0;
}
