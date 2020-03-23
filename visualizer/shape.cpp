#define GL_SILENCE_DEPRECATION

#include "shape.hpp"

#include <GL/glut.h>

#include <Eigen/Core>

#include <iostream>

namespace Shape
{

constexpr double PI = 3.14159265358979;

using namespace Eigen;

static double rad2deg(double rad)
{
    return rad / PI * 180.0;
}

inline namespace Wrapper
{
static void vertex3d(const Vector3d& pos)
{
    glVertex3f(pos(0), pos(1), pos(2));
}
static void normal3d(const Vector3d& vec)
{
    glNormal3f(vec(0), vec(1), vec(2));
}
}  // namespace Wrapper

void ShapeBase::start_draw()
{
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);

    glTranslatef(position(0), position(1), position(2));

    glRotatef(rad2deg(rotation(0)), 1.0, 0.0, 0.0);
    glRotatef(rad2deg(rotation(1)), 0.0, 1.0, 0.0);
    glRotatef(rad2deg(rotation(2)), 0.0, 0.0, 1.0);
}

void ShapeBase::end_draw()
{
    glPopMatrix();
}

void Teapot::draw() const
{
    glutSolidTeapot(size);
}

void Sphere::draw() const
{
    glutSolidSphere(radius, 20, 20);
}

void Plane::draw() const
{
    double offset = size / 2.0;
    glBegin(GL_QUADS);

    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-offset, -offset, 0);
    glVertex3f(offset, -offset, 0);
    glVertex3f(offset, offset, 0);
    glVertex3f(-offset, offset, 0);

    glEnd();
}

void Rectangular::draw() const
{
    double offset[] = {size(0) / 2., size(1) / 2., size(2) / 2.};

    glBegin(GL_QUADS);
    //前
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(offset[0], offset[1], offset[2]);
    glVertex3f(-offset[0], offset[1], offset[2]);
    glVertex3f(-offset[0], -offset[1], offset[2]);
    glVertex3f(offset[0], -offset[1], offset[2]);

    //左
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(offset[0], offset[1], offset[2]);
    glVertex3f(offset[0], offset[1], -offset[2]);
    glVertex3f(offset[0], -offset[1], -offset[2]);
    glVertex3f(offset[0], -offset[1], offset[2]);

    //右
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-offset[0], offset[1], -offset[2]);
    glVertex3f(-offset[0], offset[1], offset[2]);
    glVertex3f(-offset[0], -offset[1], offset[2]);
    glVertex3f(-offset[0], -offset[1], -offset[2]);

    //後
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(offset[0], offset[1], -offset[2]);
    glVertex3f(-offset[0], offset[1], -offset[2]);
    glVertex3f(-offset[0], -offset[1], -offset[2]);
    glVertex3f(offset[0], -offset[1], -offset[2]);

    //上
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(offset[0], offset[1], offset[2]);
    glVertex3f(-offset[0], offset[1], offset[2]);
    glVertex3f(-offset[0], offset[1], -offset[2]);
    glVertex3f(offset[0], offset[1], -offset[2]);

    //下
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(offset[0], -offset[1], offset[2]);
    glVertex3f(-offset[0], -offset[1], offset[2]);
    glVertex3f(-offset[0], -offset[1], -offset[2]);
    glVertex3f(offset[0], -offset[1], -offset[2]);
    glEnd();
}

void Cylinder::draw() const
{
    constexpr int division = 32;
    double dth = 2 * PI / division;
    const Vector3d z_offset = {0, 0, height * 0.5};

    // 側面
    glBegin(GL_QUADS);
    for (double theta = 0; theta < 2 * PI; theta += dth) {
        Vector3d v0{cos(theta), sin(theta), 0};
        Vector3d v1{cos(theta + dth), sin(theta + dth), 0};
        normal3d(0.5 * (v0 + v1));
        vertex3d(100.0 * v0 - z_offset);
        vertex3d(100.0 * v0 + z_offset);
        vertex3d(100.0 * v1 + z_offset);
        vertex3d(100.0 * v1 - z_offset);
    }
    glEnd();

    // 底面
    for (auto& top : {1, -1}) {
        glBegin(GL_TRIANGLES);
        for (double theta = 0; theta < 2 * PI; theta += dth) {
            Vector3d v[2] = {
                {cos(theta), sin(theta), 0},
                {cos(theta + dth), sin(theta + dth), 0}};
            glNormal3f(0, 0, 2 * (top - 0.5));
            vertex3d(100.0 * v[top == 1 ? 0 : 1] + top * z_offset);
            vertex3d(100.0 * v[top == 1 ? 1 : 0] + top * z_offset);
            vertex3d(top * z_offset);
        }
        glEnd();
    }
}

void Shagai::draw() const
{
    constexpr int division = 16;
    static const Vector3d center_l = {260, -40, 0};
    static const Vector3d z_offset = {0, 0, 300};

    static const double theta_s0 = PI / 2.0,
                        theta_s1 = 2 * PI - atan2(10.0 * sqrt(51), 70);
    static const double dth_s = (theta_s1 - theta_s0) / division;

    static const double theta_l0 = -PI / 2.0 - atan2(10.0 * sqrt(187), 30),
                        theta_l1 = PI / 2.0;
    static const double dth_l = (theta_l1 - theta_l0) / division;

    // 小円側面
    glBegin(GL_QUADS);
    for (double theta = theta_s0; theta < theta_s1; theta += dth_s) {
        Vector3d v0{cos(theta), sin(theta), 0};
        Vector3d v1{cos(theta + dth_s), sin(theta + dth_s), 0};
        normal3d(0.5 * (v0 + v1));
        vertex3d(100.0 * v0);
        vertex3d(100.0 * v0 + z_offset);
        vertex3d(100.0 * v1 + z_offset);
        vertex3d(100.0 * v1);
    }
    glEnd();
    // 大円側面
    glBegin(GL_QUADS);
    for (double theta = theta_l0; theta < theta_l1; theta += dth_l) {
        Vector3d v0{cos(theta), sin(theta), 0};
        Vector3d v1{cos(theta + dth_l), sin(theta + dth_l), 0};
        normal3d(0.5 * (v0 + v1));
        vertex3d(center_l + 140.0 * v0);
        vertex3d(center_l + 140.0 * v0 + z_offset);
        vertex3d(center_l + 140.0 * v1 + z_offset);
        vertex3d(center_l + 140.0 * v1);
    }
    glEnd();
    // くぼみ(右側)
    {
        Vector3d vs{cos(theta_s1), sin(theta_s1), 0};
        Vector3d vl{cos(theta_l0), sin(theta_l0), 0};

        glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        vertex3d(100.0 * vs);
        vertex3d(center_l + 140.0 * vl);
        vertex3d(center_l + 140.0 * vl + z_offset);
        vertex3d(100.0 * vs + z_offset);
        glEnd();
    }
    // くぼみ(右側)
    {
        Vector3d vs{cos(theta_s0), sin(theta_s0), 0};
        Vector3d vl{cos(theta_l1), sin(theta_l1), 0};

        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        vertex3d(center_l + 140.0 * vl);
        vertex3d(100.0 * vs);
        vertex3d(100.0 * vs + z_offset);
        vertex3d(center_l + 140.0 * vl + z_offset);
        glEnd();
    }
    for (auto& top : {1, 0}) {
        glColor3f(1.0, 1.0, 1.0 - top);
        // 小円
        glBegin(GL_TRIANGLES);
        for (double theta = theta_s0; theta < theta_s1; theta += dth_s) {
            Vector3d v[2] = {
                {cos(theta), sin(theta), 0},
                {cos(theta + dth_s), sin(theta + dth_s), 0}};
            glNormal3f(0, 0, 2 * (top - 0.5));
            vertex3d(100.0 * v[1 - top] + top * z_offset);
            vertex3d(100.0 * v[top] + top * z_offset);
            vertex3d(top * z_offset);
        }
        glEnd();
        // 大円
        glBegin(GL_TRIANGLES);
        for (double theta = theta_l0; theta < theta_l1; theta += dth_l) {
            Vector3d v[2] = {
                {cos(theta), sin(theta), 0},
                {cos(theta + dth_l), sin(theta + dth_l), 0}};
            glNormal3f(0, 0, 2 * (top - 0.5));
            vertex3d(center_l + 140.0 * v[1 - top] + top * z_offset);
            vertex3d(center_l + 140.0 * v[top] + top * z_offset);
            vertex3d(center_l + top * z_offset);
        }
        glEnd();
        // 上面
        {
            Vector3d vs{cos(theta_s0), sin(theta_s0), 0};
            Vector3d vl{cos(theta_l1), sin(theta_l1), 0};
            Vector3d x_offset[2] = {
                {0, -170, 0},
                {0, 0, 0}};

            glBegin(GL_QUADS);
            glNormal3f(0, 0, 2 * (top - 0.5));
            vertex3d(100.0 * vs + x_offset[top] + top * z_offset);
            vertex3d(100.0 * vs + x_offset[1 - top] + top * z_offset);
            vertex3d(center_l + 140.0 * vl + x_offset[1 - top] + top * z_offset);
            vertex3d(center_l + 140.0 * vl + x_offset[top] + top * z_offset);
            glEnd();
        }
    }
}  // namespace Shape

}  // namespace Shape
