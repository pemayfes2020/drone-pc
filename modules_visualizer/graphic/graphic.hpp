#pragma once

#include "color.hpp"
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

void rotateCamera(const Eigen::Vector3f& rotation);

void setWindowSize(int height, int width);
void setBGColor(Color color);

Object& addSphere(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float radius, Color color);
Object& addPlane(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float height, float width, Color color);
Object& addRectangular(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const Eigen::Vector3f& size, Color color);
Object& addCylinder(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float radius, float height, Color color);
Object& addTeapot(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, int size, Color color);
Object& addSTLModel(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const std::string filepath, bool colored, Color color);

}  // namespace Graphic
