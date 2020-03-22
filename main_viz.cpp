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

    auto ball = Graphic::addSphere(Eigen::Vector3d{100, 0, 0}, Eigen::Vector3d{0, 0, 0}, 20, Color{1.0, 0.0, 0.0});

    Graphic::start(callback);

    return 0;
}
