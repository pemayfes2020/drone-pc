#pragma once

#include <Eigen/Core>

using Color = Eigen::Vector3d;

namespace Shape
{

struct ShapeBase {
    Eigen::Vector3d position;
    Eigen::Vector3d rotation;

    ShapeBase(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation) : position(position), rotation(rotation) {}

    virtual void draw() const = 0;
    void start_draw();
    void end_draw();
};


struct Teapot : public ShapeBase {
    int size;

    Teapot(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, int size) : ShapeBase(position, rotation), size(size) {}

    virtual void draw() const override;
};

struct Sphere : public ShapeBase {
    double radius;

    Sphere(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, double radius) : ShapeBase(position, rotation), radius(radius) {}
    virtual void draw() const override;
};

struct Plane : public ShapeBase {
    double size;

    Plane(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, double size) : ShapeBase(position, rotation), size(size) {}
    virtual void draw() const override;
};


struct Rectangular : public ShapeBase {
    Eigen::Vector3d size;

    Rectangular(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, const Eigen::Vector3d& size) : ShapeBase(position, rotation), size(size) {}
    virtual void draw() const override;
};

struct Cylinder : public ShapeBase {
    double radius;
    double height;

    Cylinder(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, const double radius, const double height) : ShapeBase(position, rotation), radius(radius), height(height) {}

    virtual void draw() const override;
};

struct RugbyBall : public ShapeBase {
};

struct Shagai : public ShapeBase {
    Shagai(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation) : ShapeBase(position, rotation) {}

    virtual void draw() const override;
};

}  // namespace Shape
