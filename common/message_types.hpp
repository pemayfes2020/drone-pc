#pragma once

#include "unit.hpp"

#include <tuple>

namespace Common::Drone
{

struct ReceiveData {
    double roll;
    double pitch;
    double yaw;
    double z;
    double vx;
    double vy;
    double vz;

    ReceiveData() = default;

    ReceiveData(Angular roll, Angular pitch, Angular yaw, Length z, Vel vx, Vel vy, Vel vz)
        : roll(roll / 1.0_rad), pitch(pitch / 1.0_rad), yaw(yaw / 1.0_rad),
          z(z / 1.0_mm), vx(vx / (1.0_mm / 1.0_s)), vy(vy / (1.0_mm / 1.0_s)), vz(vz / (1.0_mm / 1.0_s))
    {
    }

    std::tuple<Angular, Angular, Angular, Length, Vel, Vel, Vel> attachUnit()
    {
        return std::make_tuple(
            roll * 1.0_rad,
            pitch * 1.0_rad,
            yaw * 1.0_rad,
            z * 1.0_mm,
            vx * 1.0_mm / 1.0_s,
            vy * 1.0_mm / 1.0_s,
            vz * 1.0_mm / 1.0_s);
    }
};

enum struct Command : char {
    TAKEOFF,
    LANDING,
    VELOCITY
};

struct SendData {
    double vx;
    double vy;
    double vz;
    double vr;
    Command command;

    SendData() = default;

    SendData(Vel vx, Vel vy, Vel vz, Vel vr, Command command = Command::VELOCITY)
        : vx(vx / (1.0_m / 1.0_s)), vy(vy / (1.0_m / 1.0_s)), vz(vz / (1.0_m / 1.0_s)), vr(vr / (1.0_m / 1.0_s))
    {
    }

    std::tuple<Vel, Vel, Vel, Vel> attachUnit()
    {
        return std::make_tuple(
            vx * 1.0_mm / 1.0_s,
            vy * 1.0_mm / 1.0_s,
            vz * 1.0_mm / 1.0_s,
            vr * 1.0_mm / 1.0_s);
    }
};

}  // namespace Common::Drone
