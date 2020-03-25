#pragma once

#include "message_types.hpp"

#include <string>

namespace Drone
{

void start(const std::string socket_path);

void send(Common::Drone::SendData send_data);

Common::Drone::ReceiveData read();

}  // namespace Drone
