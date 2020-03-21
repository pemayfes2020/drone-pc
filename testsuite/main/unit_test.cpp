#include "unit.hpp"

#include <iostream>

int main()
{

    Length x = 1.0_m, y = 11.5_mm;
    Angular theta = 1.0_rad;
    Time t = 0.1_s;

    Vel v = 1.0_m / 1.0_s;

    std::cout << "x = " << x << std::endl;
    std::cout << "v = " << v << std::endl;

    return 0;
}
