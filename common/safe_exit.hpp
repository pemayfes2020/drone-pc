#pragma once

#include <thread>

inline namespace Common
{
namespace ThreadRoom
{

void setExitHandler();

void enter();

bool toExit();

std::thread::id exit();

}  // namespace ThreadRoom
}  // namespace Common
