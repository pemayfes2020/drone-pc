#include "esp32.hpp"

#include <iostream>

int main()
{
    ESP32::start("/dev/rfcomm0");

    return 0;
}
