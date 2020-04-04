#pragma once

#include "params/environment_params.hpp"

#include "graphic/graphic.hpp"

#include <Eigen/Core>

#include <cmath>
#include <iostream>

namespace Common::Visual
{

// 静的なものと仮定して参照は返さない
class ProjectRoom
{
public:
    static void construct()
    {
        static ProjectRoom inst;
    }

private:
    ProjectRoom()
    {
        using namespace Eigen;
        using namespace Params;

        //床
        Graphic::addPlane(
            Vector3d{0.0, 0.0, 0.0},
            Vector3d{0.0, 0.0, 0.0},
            Environment::wall_width / 1.0_mm,
            Environment::wall_width / 1.0_mm,
            ColorPreset::light_gray);

        //壁
        Graphic::addPlane(
            Vector3d{0.0, -0.5 * Environment::wall_width / 1.0_mm, 0.5 * Environment::wall_width / 1.0_mm},
            Vector3d{0.5 * M_PI, 0.0, 0.0},
            Environment::wall_width / 1.0_mm,
            Environment::wall_height / 1.0_mm,
            ColorPreset::light_gray);

        Graphic::addPlane(
            Vector3d{-0.5 * Environment::wall_width / 1.0_mm, 0.0, 0.0},
            Vector3d{0.0, 0.5 * M_PI, 0.0},
            Environment::wall_height / 1.0_mm,
            Environment::wall_width / 1.0_mm,
            ColorPreset::light_gray);
    }
    ~ProjectRoom() {}
};

}  // namespace Common::Visual
