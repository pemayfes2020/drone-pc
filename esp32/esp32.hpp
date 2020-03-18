#pragma once

#include <string>

namespace ESP32
{

void start(std::string serial_port);
void setCommand(const int command);

}  // namespace ESP32
