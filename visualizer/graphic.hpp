#pragma once

#include "shape.hpp"

#include <Eigen/Core>

#include <memory>
#include <vector>


namespace Graphic
{

struct Object {
    Color color;
    std::shared_ptr<Shape::ShapeBase> shape;
};

void init(int argc, char** argv);
void start(void (*callback)(std::vector<Object>&));

void rotateCamera(const Eigen::Vector3d& rotation);

void setWindowSize(int height, int width);
void setBGColor(Color color);

Object& addSphere(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, double radius, Color color);
Object& addPlane(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, double size, Color color);
Object& addRectangular(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, const Eigen::Vector3d& size, Color color);
Object& addTeapot(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, int size, Color color);

}  // namespace Graphic
