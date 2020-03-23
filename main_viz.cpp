#include "graphic.hpp"

#include <Eigen/Core>

void callback(std::vector<Graphic::Object>& objs)
{
}

int main(int argc, char** argv)
{

    Graphic::init(argc, argv);

    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    auto drone = Graphic::addSTLModel(Eigen::Vector3f{0.0f, 0.0f, 0.0f}, Eigen::Vector3f{0.0f, 0.0f, 0.0f}, "../resource/ardrone.stl", Color{1.0, 1.0, 0.0});

    Graphic::start(callback);

    return 0;
}
