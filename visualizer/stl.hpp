#pragma once

#include <Eigen/Core>

namespace STL
{

struct Polygon {
    Eigen::Vector3f normal;
    Eigen::Vector3f r1;
    Eigen::Vector3f r2;
    Eigen::Vector3f r3;
};

}  // namespace STL

#include "stl/binary.hpp"

