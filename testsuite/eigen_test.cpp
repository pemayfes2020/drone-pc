#include "unit.hpp"

#include <Eigen/Core>

#include <iostream>

int main()
{
    Eigen::Matrix<double, 3, 1> pos{1.0, 0.0, 0.0};
    Eigen::Matrix<double, 3, 1> rot{1.0, 0.0, 0.0};

    std::cout << "pos: " << pos << std::endl;
    std::cout << "rot: " << rot << std::endl;

    return 0;
}
