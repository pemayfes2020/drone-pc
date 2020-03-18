#include "drone.hpp"
#include "safe_exit.hpp"

#include <ardrone/ardrone.h>

#include <csignal>
#include <memory>
#include <thread>

namespace Drone
{


class Drone
{
private:
    ARDrone ardrone;

public:
    Drone() : ardrone{ardrone}
    {
        // Initialize
        if (!ardrone.open()) {
            std::cerr << "[Error] [Drone] Failed to initialize." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Battery
        std::cout << "[Info] [Drone] Battery: " << ardrone.getBatteryPercentage() << "[%]" << std::endl;
    }
    ~Drone()
    {
        ardrone.close();
    }
};

void start()
{
    std::thread{
        []() {
            ThreadRoom::enter();
            Drone drone;

            while (!ThreadRoom::toExit()) {
            }

            return ThreadRoom::exit();
        }}
        .detach();
}

//検出した円の中心座標、半径を標準出力で確認する用
void drawCircles(std::vector<double> circles)
{
    if (circles.size()) {
        std::cout << std::endl;
        for (size_t i = 0; i < circles.size(); i++) {
            std::cout << "r = " << circles[i] << std::endl;
        }
        std::cout << std::endl;
    }
}

}  // namespace Drone
