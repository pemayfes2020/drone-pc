#include "visual/project_room.hpp"
#include "visual/drone.hpp"

#include "params/environment_params.hpp"
#include "unit.hpp"

#include <graphic/graphic.hpp>

#include <Eigen/Core>

#include <cmath>
#include <iostream>

Eigen::Vector3d pos;
Eigen::Vector3d rot;

void callback(std::vector<Graphic::Object>& objs)
{
    using Freq = decltype(1.0 / 1.0_s);

    static Time t = 0.0_s;
    static const Freq freq = 0.1 / 1.0_s;
    static const Length amp = 0.5 * Params::Environment::wall_width;

    pos(0) = amp * std::sin(2.0 * M_PI * freq * t) / 1.0_mm;
    Common::Visual::Drone::update(pos, rot);

    std::cout << t << ' ' << pos(0) << std::endl;
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
