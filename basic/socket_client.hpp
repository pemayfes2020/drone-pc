#pragma once

#include <string>

namespace UnixSocketClient
{

void init(const std::string& bind_path);

void write(const std::string& str);
std::string read();

}  // namespace UnixSocketClient
