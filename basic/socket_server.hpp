#pragma once

#include <string>

namespace UnixSocketServer
{

void init(const std::string& bind_path);

void write(const std::string& str);
std::string read();

}  // namespace UnixSocketServer
