#include "visual/project_room.hpp"
#include "visual/drone.hpp"

#include "params/environment_params.hpp"
#include "unit.hpp"

#include <graphic/graphic.hpp>

#include <Eigen/Core>

#include <cmath>
#include <iostream>
#include <memory>

Eigen::Vector3d pos{0.0, 0.0, 0.0};
Eigen::Vector3d rot{0.0, 0.0, 0.0};

void callback()
{
    using Freq = decltype(1.0 / 1.0_s);

    static Time t = 0.0_s;
    static const Freq freq = 0.1 / 1.0_s;
    static const Length amp_x = 0.4 * Params::Environment::wall_width;
    static const Angular amp_roll = 0.2 * M_PI * 1.0_rad;
    static const AngularVel vel_yaw = M_PI * 1.0_rad / 1.0_s;

    pos(0) = amp_x * std::sin(2.0 * M_PI * freq * t) / 1.0_mm;
    pos(2) = 500.0;
    rot(0) = amp_roll * std::sin(2.0 * M_PI * freq * t) / 1.0_rad;
    rot(2) = vel_yaw * t / 1.0_rad;
    Common::Visual::Drone::update(pos, rot);

    Graphic::setCameraTarget(pos);

    t += 0.03_s;
}

int main(int argc, char** argv)
{
    Graphic::init(argc, argv);
    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(ColorPreset::dark_gray);

    Common::Visual::ProjectRoom::construct();
    Common::Visual::Drone::construct("../resource/ardrone.stl");

    Graphic::start(callback);

    return 0;
}
