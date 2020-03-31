#pragma once

#include "color.hpp"

#include <Eigen/Core>

namespace STL
{

struct Polygon {
    Eigen::Vector3f normal;
    Eigen::Vector3f r1;
    Eigen::Vector3f r2;
    Eigen::Vector3f r3;
};

struct ColoredPolygon {
    Eigen::Vector3f normal;
    Eigen::Vector3f r1;
    Eigen::Vector3f r2;
    Eigen::Vector3f r3;
    bool unique;
    Color color;
};

}  // namespace STL

#include "stl/binary.hpp"
#include "stl/color.hpp"
