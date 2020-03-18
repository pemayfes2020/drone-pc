#include "esp32.hpp"
#include "safe_exit.hpp"

#include <csignal>
#include <iostream>

int main()
{
    ThreadRoom::setExitHandler();

    ESP32::start("/dev/rfcomm0");


    return 0;
}
