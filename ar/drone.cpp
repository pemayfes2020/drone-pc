#include "safe_exit.hpp"

#include <ardrone/ardrone.h>

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
            try {
                ThreadRoom::enter();
                Drone drone;

                while (!ThreadRoom::toExit()) {
                }

            } catch (ThreadRoom::thread_abort) {
                ThreadRoom::exit();
                return;
            }
        }}
        .detach();
}

}  // namespace Drone
