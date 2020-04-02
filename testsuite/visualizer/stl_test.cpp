#include "graphic/graphic.hpp"

#include <Eigen/Core>

#include <cmath>

Eigen::Vector3d pos;
Eigen::Vector3d rot;

Graphic::Object drone;
Graphic::Object sphere;

void callback(std::vector<Graphic::Object>& objs)
{
    drone.pos = pos;
    drone.rot = rot;
}

int main(int argc, char** argv)
{
    Graphic::init(argc, argv);

    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    pos << 0.0, 0.0, 0.0;
    rot << 0.0, 0.0, 0.0;

    drone = Graphic::addSTLModel(pos, rot, "../resource/ardrone.stl", true, Color{1.0, 1.0, 0.0});
    Graphic::addSTLModel(pos, rot, "../resource/human.stl", true, Color{1.0, 1.0, 0.0});

    Graphic::start(callback);

    return 0;
}
