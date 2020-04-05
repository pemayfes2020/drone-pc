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

class InputWrapper
{
    bool offline;
    std::unique_ptr<UnixSocket::Server> server;
    std::unique_ptr<std::ofstream> log;


public:
    InputWrapper(bool offline, std::string filepath)
        : offline{offline},
          server{offline ? nullptr : std::make_unique<UnixSocket::Server>(filepath)},
          log{offline ? std::make_unique(filepath) : nullptr}
    {
    }

    InputWrapper& operator>>(Common::Visualizer::VectorData& data)
    {
        if (offline) {
            constexpr int size = sizeof(Common::Visualizer::VectorData);
            union {
                Common::Visualizer::VectorData& data;
                char bytes[size];
            } buf;
            log->read(buf.bytes, size);
            data = buf.data;
        } else {
            data = server->read<Common::Visualizer::VectorData>();
        }
        return *this;
    }
};

int main(int argc, char** argv)
{
    cmdline::parser parser;
    parser.add<std::string>("socket", 's', "unix domain socket", false, "/tmp/viz.sock");
    parser.add<std::string>("model", 'm', "Drone STL Model File", false, "../resource/ardrone.stl");
    parser.add<std::string>("log-file", 'f', "Log file to visualize", false, "");
    parser.add("offline", 'o', "Offline visualize with log data");

    signal(SIGINT, sigint_handler);

    bool offline = parser.exist("offline");
    InputWrapper data_input{offline, offline ? parser.get<std::string>("log-file") : parser.get<std::string>("socket")};

    std::thread comm_thread{
        [&exit_flag, &pos, &rot](InputWrapper& input) {
            Common::Visualizer::VectorData data;
            while (!exit_flag) {
                input >> data;
                pos << data.x, data.y, data.z;
                rot << data.roll, data.pitch, data.yaw;
            }
        },
        std::ref(data_input)};

    Graphic::init(argc, argv);

    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    Common::Visual::ProjectRoom::construct();
    Common::Visual::Drone::construct(parser.get<std::string>("model"));

    Graphic::start(callback);

    comm_thread.join();

    return 0;
}
