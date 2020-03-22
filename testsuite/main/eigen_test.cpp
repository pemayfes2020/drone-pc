#include "unit.hpp"

#include <Eigen/Core>

#include <iostream>

int main()
{
    Eigen::Matrix<double, 3, 1> pos{1.0, 0.0, 0.0};
    Eigen::Matrix<double, 3, 1> rot{1.0, 0.0, 0.0};

    Eigen::Matrix<double, 3, 3> A;
    A << 1.0, 0.0, 2.0,
        3.0, 4.0, 5.0,
        6.0, 6.0, 7.0;

    Eigen::Matrix<double, 3, 1> b = A * pos;

    std::cout << "pos: " << pos << std::endl;
    std::cout << "rot: " << rot << std::endl;

    return 0;
}
