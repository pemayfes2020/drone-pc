#include "unit.hpp"

#include "control_params.hpp"

#include <Eigen/Core>

namespace Control
{

Eigen::Matrix<double, 3, 9> params()
{
    using namespace Params;
    using CVec6d = Eigen::Matrix<double, 1, 6>;
    using CVec3d = Eigen::Matrix<double, 1, 3>;

    static const Eigen::Matrix<double, 3, 9> pid_params =
        []() -> Eigen::Matrix<double, 3, 9> {
        Eigen::Matrix<double, 3, 9> mat;
        mat << Kp_x, Ki_x, Kd_x, CVec6d::Zero(),
            CVec3d::Zero(), Kp_y, Ki_y, Kd_y, CVec3d::Zero(),
            CVec6d::Zero(), Kp_z, Ki_z, Kd_z;
        /*
         * Kp_x, Ki_x, Kd_x, 0, 0, 0, 0, 0, 0
         * 0, 0, 0, Kp_x, Ki_x, Kd_x, 0, 0, 0
         * 0, 0, 0, 0, 0, 0, Kp_x, Ki_x, Kd_x
         */
    }();

    return pid_params;
};

static Eigen::Vector3d p_val_prev;
static Eigen::Vector3d i_val;

std::array<Vel, 3> control(
    std::array<Length, 3> pos,
    std::array<Length, 3> dest)
{
    Eigen::Vector3d p_val;
    p_val << (dest[0] - pos[0]) / 1.0_mm,
        (dest[1] - pos[1]) / 1.0_mm,
        (dest[2] - pos[2]) / 1.0_mm;

    Eigen::Vector3d d_val = p_val_prev - p_val;
    i_val += p_val;

    Eigen::Matrix<double, 9, 1> pid_val;
    pid_val << p_val, i_val, d_val;

    p_val_prev = p_val;

    Eigen::Vector3d output = params() * pid_val;

    return std::array<Vel, 3>{
        output(0) * 1.0_mm / 1.0_s,
        output(2) * 1.0_mm / 1.0_s,
        output(3) * 1.0_mm / 1.0_s};
}

}  // namespace Control
