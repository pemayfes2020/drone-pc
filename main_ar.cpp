#include "message_types.hpp"
#include "socket.hpp"
#include "unit.hpp"

#include <ardrone/ardrone.h>

#include <opencv/cv.hpp>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

void init(ARDrone& ardrone)
{
    // Initialize
    if (!ardrone.open()) {
        std::cerr << "[Error] [Drone] Failed to initialize." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Info
    int major_version, minor_version, revision;
    ardrone.getVersion(&major_version, &minor_version, &revision);
    std::cout << "[Info] [Drone] Version: " << major_version << '.' << minor_version << '.' << revision << std::endl;
    std::cout << "[Info] [Drone] Battery: " << ardrone.getBatteryPercentage() << "[%]" << std::endl;
}

using namespace Common;

constexpr int timeout_deadline = 1500;
bool timeout(std::chrono::system_clock::time_point start)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() < timeout_deadline;
}

void command(ARDrone& ardrone, Drone::SendData message);

int main()
{
    UnixSocket::Server server{"/tmp/ar.sock"};

    ARDrone ardrone;
    init(ardrone);

    bool connected = false;  // timeoutは2回目以降にかける

    cv::Mat image;
    constexpr int front_camera = 0, lower_camera = 1;
    ardrone.setCamera(lower_camera);

    while (ardrone.update()) {
        // Receive Command from main
        Drone::SendData message;
        if (not connected) {
            message = server.read<Drone::SendData>();
            connected = true;
        } else {
            // timeout処理
            volatile std::atomic end_flag = false;
            std::thread thread_read{
                [&server, &end_flag](Drone::SendData& ref) {
                    ref = server.read<Drone::SendData>();
                    end_flag = true;
                },
                std::ref(message)};

            auto start = std::chrono::system_clock::now();
            while (!end_flag or timeout(start)) {
            }
            if (!end_flag) {
                ardrone.landing();
                std::exit(EXIT_FAILURE);
            }
            thread_read.join();
        }
        command(ardrone, message);

        // Get Sensor Data
        Angular roll = ardrone.getRoll() * 1.0_rad,
                pitch = ardrone.getPitch() * 1.0_rad,
                yaw = ardrone.getYaw() * 1.0_rad;
        Length z = ardrone.getAltitude() * 1.0_m;
        double _vx, _vy, _vz;
        ardrone.getVelocity(&_vx, &_vy, &_vz);
        Vel vx = _vx * 1.0_m / 1.0_s,
            vy = _vx * 1.0_m / 1.0_s,
            vz = _vx * 1.0_m / 1.0_s;
        ardrone >> image;

        // Send Sensor Data to main
        server.write<Drone::ReceiveData>(Drone::ReceiveData{roll, pitch, yaw, z, vx, vy, vz});
    }

    ardrone.close();

    return 0;
}

void command(ARDrone& ardrone, Drone::SendData message)
{
    switch (message.command) {
    case Drone::Command::TAKEOFF:
        ardrone.takeoff();
        break;
    case Drone::Command::LANDING:
        ardrone.landing();
        break;
    case Drone::Command::VELOCITY:
        ardrone.move3D(message.vx, message.vy, message.vz, message.vr);
        break;
    }
}
