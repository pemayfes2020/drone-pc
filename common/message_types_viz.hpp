#pragma once

#include "unit.hpp"

#include <tuple>

namespace Common::Visualizer
{

struct VectorData {

    double t;

    double x;
    double y;
    double z;

    double roll;
    double pitch;
    double yaw;

    VectorData() = default;

    VectorData(Time t, Length x, Length y, Length z, Angular roll, Angular pitch, Angular yaw)
        : t(t / 1.0_s), x(x / 1.0_mm), y(y / 1.0_mm), z(z / 1.0_mm),
          roll(roll / 1.0_rad), pitch(pitch / 1.0_rad), yaw(yaw / 1.0_rad)
    {
    }

    std::tuple<Time, Length, Length, Length, Angular, Angular, Angular> attachUnit()
    {
        return std::make_tuple(
            t * 1.0_s,
            x * 1.0_mm,
            y * 1.0_mm,
            z * 1.0_mm,
            roll * 1.0_rad,
            pitch * 1.0_rad,
            yaw * 1.0_rad);
    }
    friend std::ostream& operator<<(std::ostream& out, const VectorData& data);
};

std::ostream& operator<<(std::ostream& out, const VectorData& data)
{
    out << data.t << ' '
        << data.x << ' '
        << data.y << ' '
        << data.z << ' '
        << data.roll << ' '
        << data.pitch << ' '
        << data.yaw;
    return out;
}

}  // namespace Common::Visualizer
