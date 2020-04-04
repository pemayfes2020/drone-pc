#pragma once

#include "params/environment_params.hpp"

#include "graphic/graphic.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <cmath>
#include <iostream>
#include <memory>
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
    Graphic::ObjectId ball;
    Graphic::ObjectId body;

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
        using namespace Eigen;

        Graphic::setPosition(body, pos);
        Graphic::setRotation(body, rot);

        static const Vector3d offset{
            0.0, 0.0, Params::Environment::ball_offset / 1.0_mm};
        Matrix3d attitude
            = (AngleAxisd(rot(0), Vector3d::UnitX())
                * AngleAxisd(rot(1), Vector3d::UnitY())
                * AngleAxisd(rot(2), Vector3d::UnitZ()))
                  .toRotationMatrix();
        Graphic::setPosition(ball, pos + attitude * offset);
        Graphic::setRotation(ball, Vector3d::Zero());
    }
};  // namespace Common::Visual


}  // namespace Common::Visual
