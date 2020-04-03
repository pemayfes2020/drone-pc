#include "visual/project_room.hpp"
#include "visual/drone.hpp"

#include "params/environment_params.hpp"
#include "unit.hpp"

#include <graphic/graphic.hpp>

#include <Eigen/Core>

#include <cmath>
#include <iostream>

Eigen::Vector3d pos{0.0, 0.0, 0.0};
Eigen::Vector3d rot{0.0, 0.0, 0.0};

void callback(std::vector<Graphic::Object>& objs)
{
    using Freq = decltype(1.0 / 1.0_s);

    static Time t = 0.0_s;
    static const Freq freq = 0.1 / 1.0_s;
    static const Length amp = 0.4 * Params::Environment::wall_width;

    pos(0) = amp * std::sin(2.0 * M_PI * freq * t) / 1.0_mm;
    Common::Visual::Drone::update(pos, rot);

    std::cout << "obj(-1): " << std::hex << objs.at(objs.size() - 1).shape.get() << std::endl;
    std::cout << "obj(-2): " << std::hex << objs.at(objs.size() - 2).shape.get() << std::endl;

    // std::cout << t << ' ' << pos(0) * 1.0_mm << std::endl;
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
