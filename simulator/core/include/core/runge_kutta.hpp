#pragma once

#include "unit.hpp"

#include <functional>
#include <type_traits>

namespace Core
{

namespace Impl
{
template <class D>
using U = unit_type<D>;

template <class D1, class D2>
using div_type = decltype(U<D2>{} / U<D1>{});

/*
template <class D1, class D2>
using diff_func_t = std::function<div_type<D1, D2>(D1, D2)>;

template <class D1, class D2>
using diff_ref_t = div_type<D1, D2> (&)(U<D1>, U<D2>);
*/

template <class D1, class D2, class F>
constexpr bool is_diff_func = std::is_invocable_r<div_type<D1, D2>, F, U<D1>, U<D2>>::value;

template <class D1, class D2, class F>
using diff_func_t = typename std::enable_if<is_diff_func<D1, D2, F>>::type;

template <class D1, class D2, class F>
using not_diff_func_t = typename std::enable_if<!is_diff_func<D1, D2>, F>::type;

}  // namespace Impl

template <class D1, class D2, class diff_type, Impl::diff_func_t<D1, D2, diff_type>* = nullptr>
Impl::U<D2> runge_kutta(Impl::U<D1> x0, Impl::U<D2> y0, diff_type dydx, Impl::U<D1> step)
{
    decltype(y0) k1 = step * dydx(x0, y0);
    decltype(y0) k2 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k1);
    decltype(y0) k3 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k2);
    decltype(y0) k4 = step * dydx(x0 + step, y0 + k3);

    return (k1 + 2.0 * (k2 + k3) + k4) / 6.0;
}

template <class D1, class D2, class diff_type, Impl::not_diff_func_t<D1, D2, diff_type>* = nullptr>
Impl::U<D2> runge_kutta(Impl::U<D1> x0, Impl::U<D2> y0, diff_type dydx, Impl::U<D1> step)
{
    //static_assert(false, "Specified Diff Func is not a valid type");
}


/*
template <class D1, class D2>
auto runge_kutta(Impl::U<D1> x0, Impl::U<D2> y0, Impl::diff_func_t<D1, D2> dydx, Impl::U<D1> step)
{
    decltype(y0) k1 = step * dydx(x0, y0);
    decltype(y0) k2 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k1);
    decltype(y0) k3 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k2);
    decltype(y0) k4 = step * dydx(x0 + step, y0 + k3);

    return (k1 + 2.0 * (k2 + k3) + k4) / 6.0;
}

template <class D1, class D2>
auto runge_kutta(Impl::U<D1> x0, Impl::U<D2> y0, Impl::diff_ref_t<D1, D2> dydx, Impl::U<D1> step)
{
    decltype(y0) k1 = step * dydx(x0, y0);
    decltype(y0) k2 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k1);
    decltype(y0) k3 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k2);
    decltype(y0) k4 = step * dydx(x0 + step, y0 + k3);

    return (k1 + 2.0 * (k2 + k3) + k4) / 6.0;
}

template <class D1, class D2>
auto runge_kutta(Impl::U<D1> x0, Impl::U<D2> y0, Impl::diff_lambda_t<D1, D2> dydx, Impl::U<D1> step)
{
    decltype(y0) k1 = step * dydx(x0, y0);
    decltype(y0) k2 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k1);
    decltype(y0) k3 = step * dydx(x0 + 0.5 * step, y0 + 0.5 * k2);
    decltype(y0) k4 = step * dydx(x0 + step, y0 + k3);

    return (k1 + 2.0 * (k2 + k3) + k4) / 6.0;
}
*/


}  // namespace Core
