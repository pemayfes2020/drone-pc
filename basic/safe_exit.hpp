#pragma once

#include <thread>

inline namespace Basic
{
namespace ThreadRoom
{

struct thread_abort {
};

void setExitHandler();

void enter();

bool toExit();

std::thread::id exit();

}  // namespace ThreadRoom
}  // namespace Basic
