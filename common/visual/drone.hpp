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
        return inst;
    }

    static void update(Eigen::Vector3d pos, Eigen::Vector3d rot)
    {
        construct()._update(pos, rot);
    }

private:
    Graphic::Object& ball;
    Graphic::Object& body;

    Drone(std::string filepath)
        : ball{Graphic::addSphere(
              Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(),
              Params::Environment::ball_radius / 1.0_mm,
              Color{0.0, 0.2, 0.8})},
          body{Graphic::addSTLModel(
              Params::Environment::initial_pos,
              Params::Environment::initial_rot,
              filepath, true, ColorPreset::empty)}
    {
    }

    void _update(Eigen::Vector3d pos, Eigen::Vector3d rot)
    {
        body.pos = pos;
        body.rot = rot;

        // TODO 要修正
        ball.pos = pos;
        ball.rot = rot;
    }
};


}  // namespace Common::Visual
