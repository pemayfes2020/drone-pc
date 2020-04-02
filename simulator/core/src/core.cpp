#include "core/message_types.hpp"
#include "core/model.hpp"
#include "core/runge_kutta.hpp"

#include "shared_memory.hpp"
#include "unit.hpp"

#include <functional>

namespace Core
{


}  // namespace Core

int main()
{
    Core::Model drone;
    SharedMemory::ReadWrite<Core::StateData> shm(Core::state_shm_name);
}

