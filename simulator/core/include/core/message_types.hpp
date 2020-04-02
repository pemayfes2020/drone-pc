#pragma once

#include "params/kinect_params.hpp"

#include <Eigen/Core>

namespace Core
{

inline const char* state_shm_name = "StateData";
struct StateData {
    Eigen::Vector3d pos;
    Eigen::Vector3d rot;
};

inline const char* kinect_shm_name = "KinectData";
struct KinectData {
    Params::Kinect::RGB::pixel_type rgb[Params::Kinect::RGB::size];
    Params::Kinect::Ir::pixel_type ir[Params::Kinect::Ir::size];
    Params::Kinect::Depth::pixel_type depth[Params::Kinect::Depth::size];
};

}  // namespace Core
