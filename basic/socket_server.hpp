#pragma once

#include <string>

inline namespace Basic
{
namespace UnixSocketServer
{

void init(const std::string& bind_path);

void write(const std::string& str);
std::string read();

}  // namespace UnixSocketServer
}  // namespace Basic
