#include "drone.hpp"
#include "safe_exit.hpp"

#include <memory>
#include <thread>

namespace Drone
{


class Drone
{
private:
public:
    Drone()
    {
    }
    ~Drone()
    {
    }
};

void start()
{
    std::thread{
        []() {
            try {
                ThreadRoom::enter();

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
