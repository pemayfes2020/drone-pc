#include "graphic/graphic.hpp"

#include <Eigen/Core>

#include <cmath>

Eigen::Vector3f pos;
Eigen::Vector3f rot;

Graphic::Object drone;
Graphic::Object sphere;

void callback(std::vector<Graphic::Object>& objs)
{
    drone.shape->position = pos;
    drone.shape->rotation = rot;
}

int main(int argc, char** argv)
{
    Graphic::init(argc, argv);

    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    pos << 0.0f, 0.0f, 0.0f;
    rot << 0.0f, 0.0f, 0.0f;

    drone = Graphic::addSTLModel(pos, rot, "../resource/ardrone.stl", Color{1.0, 1.0, 0.0});

    Graphic::start(callback);

    return 0;
}
