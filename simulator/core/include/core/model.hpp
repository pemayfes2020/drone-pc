#pragma once

#include "core/runge_kutta.hpp"
#include "params/simulator_params.hpp"

#include "unit.hpp"

#include <Eigen/Core>

#include <chrono>
#include <cmath>

namespace Core
{

class Model
{
    Eigen::Vector3d pos;  // フィールド座標系での位置
    Eigen::Vector3d rot;  // フィールド座標系での角度

    Eigen::Vector3d vel;          // ドローン座標系でのd/dt(x, y, z)
    Eigen::Vector3d angular_vel;  // ドローン座標系でのd/dt(roll, pitch, yaw)

    Eigen::Vector3d target_vel;          // ドローン座標系でのd/dt(x, y, z)
    Eigen::Vector3d target_angular_vel;  // ドローン座標系でのd/dt(roll, pitch, yaw)

    std::chrono::system_clock::time_point last_update;

    bool started;   // takeoffのスタート
    bool onflight;  // takeoffが済んでいるか

public:
    Model() : pos(0.0, 0.0, 0.0), rot(0.0, 0.0, 0.0), last_update(std::chrono::system_clock::now()) {}
    ~Model() {}

    void takeoff()
    {
        started = true;
    }

    void setVelocity(Vel vx, Vel vy, Vel vz, AngularVel vr)
    {
        target_vel << vx / (1.0_mm / 1.0_s), vy / (1.0_mm / 1.0_s), vz / (1.0_mm / 1.0_s);
        angular_vel(2) = vr / (1.0_rad / 1.0_s);
    }

    void update()
    {
        auto now = std::chrono::system_clock::now();
        Time duration = std::chrono::duration_cast<std::chrono::microseconds>(now - last_update).count() * 1.0 * 1.0e-6_s;
        Time dt = duration / (double)Params::Simulator::update_division;
        if (!onflight) {
            pos(2) += Params::Simulator::takeoff_vz * duration / 1.0_mm;
            if (pos(2) > Params::Simulator::takeoff_z_threshold / 1.0_mm) {
                onflight = true;
            }
        } else {
            for (double t = 0; t < duration / 1.0_s; t += dt / 1.0_s) {
                vel(0) += (target_vel(0) - vel(0)) * (1.0 - std::exp(dt / Params::Simulator::vz_time_constant));
                vel(1) += (target_vel(1) - vel(1)) * (1.0 - std::exp(dt / Params::Simulator::vxy_time_constant));
                vel(2) += (target_vel(2) - vel(2)) * (1.0 - std::exp(dt / Params::Simulator::vxy_time_constant));
                angular_vel(2) += (target_angular_vel(2) - angular_vel(2)) * (1.0 - std::exp(dt / Params::Simulator::vr_time_constant));
                pos += vel * (dt / 1.0_s);
                rot(2) += angular_vel(2) * dt / 1.0_s;
            }
        }
    }
};

}  // namespace Core
