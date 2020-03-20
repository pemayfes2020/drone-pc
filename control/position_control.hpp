#pragma once

#include "unit.hpp"

#include <array>

namespace Control
{

std::array<Vel, 3, 1> control(std::array<Length, 3, 1> pos, std::array<Length, 3, 1> dest);


}  // namespace Control
