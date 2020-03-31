#include "core/runge_kutta.hpp"

#include "unit.hpp"

#include <functional>


decltype(1.0 / Time{}) k = 1.0 / 1.0_s;
Vel diff(Time _t, Length _x) { return -k * _x; }

void hoge()
{
    Length x;
    Time t;

    /*
    auto func
        = [&k](Time t, Length x) -> Vel { return -k * x; };
    */

    x += Core::runge_kutta(t, x, diff, 0.01_s);
}
