#include "unit.hpp"

#include <iostream>

int main()
{

    SI::Length x = 1.0_m, y = 11.5_mm;
    SI::Angular theta = 1.0_rad;
    SI::Time t = 0.1_s;

    SI::Vel v = 1.0_m / 1.0_s;

    std::cout << "x = " << x << std::endl;
    std::cout << "v = " << v << std::endl;

    return 0;
}
