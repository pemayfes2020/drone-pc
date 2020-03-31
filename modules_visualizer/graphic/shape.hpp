#pragma once

#include "stl.hpp"

#include <Eigen/Core>

using Color = Eigen::Vector3f;

namespace Shape
{

struct ShapeBase {
    Eigen::Vector3f position;
    Eigen::Vector3f rotation;

    ShapeBase(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation) : position(position), rotation(rotation) {}

    virtual void draw() const = 0;
    void start_draw();
    void end_draw();
};


struct Teapot : public ShapeBase {
    int size;

    Teapot(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, int size) : ShapeBase(position, rotation), size(size) {}

    virtual void draw() const override;
};

struct Sphere : public ShapeBase {
    float radius;

    Sphere(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float radius) : ShapeBase(position, rotation), radius(radius) {}
    virtual void draw() const override;
};

struct Plane : public ShapeBase {
    float height;
    float width;

    Plane(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float height, float width) : ShapeBase(position, rotation), height(height), width(width) {}
    virtual void draw() const override;
};


struct Rectangular : public ShapeBase {
    Eigen::Vector3f size;

    Rectangular(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const Eigen::Vector3f& size) : ShapeBase(position, rotation), size(size) {}
    virtual void draw() const override;
};

struct Cylinder : public ShapeBase {
    float radius;
    float height;

    Cylinder(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const float radius, const float height) : ShapeBase(position, rotation), radius(radius), height(height) {}

    virtual void draw() const override;
};

struct STLModel : public ShapeBase {
    STL::Binary data;

    STLModel(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const std::string filepath) : ShapeBase(position, rotation), data(filepath) {}
    virtual void draw() const override;
};

struct ColoredSTLModel : public ShapeBase {
    STL::ColorBinary data;

    ColoredSTLModel(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const std::string filepath) : ShapeBase(position, rotation), data(filepath) {}
    virtual void draw() const override;
};


}  // namespace Shape
