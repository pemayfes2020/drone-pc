#pragma once

#include "unit.hpp"

#include <tuple>

namespace Common::Visualizer
{

struct VectorData {

    double x;
    double y;
    double z;

    double roll;
    double pitch;
    double yaw;

    VectorData() = default;

    VectorData(Length x, Length y, Length z, Angular roll, Angular pitch, Angular yaw)
        : x(x / 1.0_mm), y(y / 1.0_mm), z(z / 1.0_mm),
          roll(roll / 1.0_rad), pitch(pitch / 1.0_rad), yaw(yaw / 1.0_rad)
    {
    }

    std::tuple<Length, Length, Length, Angular, Angular, Angular> attachUnit()
    {
        return std::make_tuple(
            x * 1.0_mm,
            y * 1.0_mm,
            z * 1.0_mm,
            roll * 1.0_rad,
            pitch * 1.0_rad,
            yaw * 1.0_rad);
    }
};

}  // namespace Common::Visualizer
