#pragma once

#include "unit.hpp"

namespace Common
{


struct DroneReceiveData {
    double roll;
    double pitch;
    double yaw;
    double z;
    double vx;
    double vy;
    double vz;

    DroneReceiveData() = default;

    DroneReceiveData(Angular roll, Angular pitch, Angular yaw, Length z, Vel vx, Vel vy, Vel vz)
        : roll(roll / 1.0_rad), pitch(pitch / 1.0_rad), yaw(yaw / 1.0_rad),
          z(z / 1.0_mm), vx(vx / (1.0_mm / 1.0_s)), vy(vy / (1.0_mm / 1.0_s)), vz(vz / (1.0_mm / 1.0_s))
    {
    }
};

struct DroneSendData {
    double vx;
    double vy;
    double vz;
};

}  // namespace Common
