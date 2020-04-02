#pragma once

#include "params/environment_params.hpp"

#include "graphic/graphic.hpp"

#include <Eigen/Core>

#include <cmath>
#include <iostream>
#include <string>

namespace Common::Visual
{

class Drone
{
public:
    static Drone& construct(const std::string& filepath = "")
    {
        static Drone inst{filepath};
    }

    static void update(Eigen::Vector3d pos, Eigen::Vector3d rot)
    {
        construct()._update(pos, rot);
    }

private:
    Graphic::Object body;
    Graphic::Object ball;

    Drone(std::string filepath) : body{}, ball{}
    {
        using namespace Eigen;
        using namespace Params;

        Vector3d zero = Vector3d::Zero();

        body = Graphic::addSTLModel(Environment::initial_pos, Environment::initial_rot, filepath, true, ColorPreset::empty);
        ball = Graphic::addSphere(zero, zero, Environment::ball_radius / 1.0_mm, Color{0.0, 0.2, 0.8});
    }

    void _update(Eigen::Vector3d pos, Eigen::Vector3d rot)
    {
        body.pos = pos;
        body.rot = rot;

        // TODO 要修正
        ball.pos = pos;
        body.rot = rot;
    }
};


}  // namespace Common::Visual
