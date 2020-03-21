#include "drone.hpp"
#include "safe_exit.hpp"

#include <iostream>

int main()
{
    ThreadRoom::setExitHandler();

    Drone::start();

    while (true) {
    }

    return 0;
}
