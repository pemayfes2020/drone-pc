#pragma once

#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>

inline namespace Common
{


// Unit Types
using Length = boost::units::quantity<boost::units::si::length, double>;
using Vel = boost::units::quantity<boost::units::si::velocity, double>;
using Acc = boost::units::quantity<boost::units::si::acceleration, double>;
using Angular = boost::units::quantity<boost::units::si::plane_angle, double>;
using AngularVel = boost::units::quantity<boost::units::si::angular_velocity, double>;
using AngularAcc = boost::units::quantity<boost::units::si::angular_acceleration, double>;
using Time = boost::units::quantity<boost::units::si::time, double>;

namespace SI
{

// Unit Constants
inline const Length& meter = 1.0 * boost::units::si::meter;
inline const Length& millimeter = 1.0e-3 * boost::units::si::meter;
inline const Time& second = 1.0 * boost::units::si::second;
inline const Angular& rad = 1.0 * boost::units::si::radian;

}  // namespace SI

inline namespace SILiteral
{

template <class T>
struct value_type {
    using type = T;
};

template <class D, class T>
struct value_type<boost::units::quantity<D, T>> {
    using type = T;
};

#define DECLARE_LITERAL(unit_type, constant, literal)                        \
    inline unit_type operator""_##literal(long double value)                 \
    {                                                                        \
        return static_cast<value_type<unit_type>::type>(value) * (constant); \
    }

DECLARE_LITERAL(Length, SI::meter, m)
DECLARE_LITERAL(Length, SI::millimeter, mm)
DECLARE_LITERAL(Time, SI::second, s)
DECLARE_LITERAL(Angular, SI::rad, rad)


}  // namespace SILiteral

}  // namespace Common
