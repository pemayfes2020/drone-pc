#define GL_SILENCE_DEPRECATION
#pragma push
#pragma GCC diagnostic ignored "-Wnarrowing"

#include "graphic/shape.hpp"
#include "graphic/stl.hpp"

#include <Eigen/Core>
#include <GL/glut.h>

#include <iostream>

namespace Shape
{

constexpr float PI = 3.14159265358979f;

using namespace Eigen;

static float rad2deg(float rad)
{
    return rad / PI * 180.0;
}

inline namespace Wrapper
{
static void vertex3f(const Vector3f& pos)
{
    glVertex3f(pos(0), pos(1), pos(2));
}
static void normal3f(const Vector3f& vec)
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
    float hei = height / 2.0;
    float wid = width / 2.0;
    glBegin(GL_QUADS);


    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-hei, -wid, 0);
    glVertex3f(hei, -wid, 0);
    glVertex3f(hei, wid, 0);
    glVertex3f(-hei, wid, 0);

    glEnd();
}

void Rectangular::draw() const
{
    float offset[] = {size(0) / 2., size(1) / 2., size(2) / 2.};

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
    float dth = 2 * PI / division;
    const Vector3f z_offset = {0, 0, height * 0.5};

    // 側面
    glBegin(GL_QUADS);
    for (float theta = 0; theta < 2 * PI; theta += dth) {
        Vector3f v0{cos(theta), sin(theta), 0};
        Vector3f v1{cos(theta + dth), sin(theta + dth), 0};
        normal3f(0.5 * (v0 + v1));
        vertex3f(100.0 * v0 - z_offset);
        vertex3f(100.0 * v0 + z_offset);
        vertex3f(100.0 * v1 + z_offset);
        vertex3f(100.0 * v1 - z_offset);
    }
    glEnd();

    // 底面
    for (auto& top : {1, -1}) {
        glBegin(GL_TRIANGLES);
        for (float theta = 0; theta < 2 * PI; theta += dth) {
            Vector3f v[2] = {
                {cos(theta), sin(theta), 0},
                {cos(theta + dth), sin(theta + dth), 0}};
            glNormal3f(0, 0, 2 * (top - 0.5));
            vertex3f(100.0 * v[top == 1 ? 0 : 1] + top * z_offset);
            vertex3f(100.0 * v[top == 1 ? 1 : 0] + top * z_offset);
            vertex3f(top * z_offset);
        }
        glEnd();
    }
}

void STLModel::draw() const
{
    glBegin(GL_TRIANGLES);
    for (auto polygon : data.polygons) {
        normal3f(polygon.normal);
        vertex3f(polygon.r1);
        vertex3f(polygon.r2);
        vertex3f(polygon.r3);
    }
    glEnd();
}

void ColoredSTLModel::draw() const
{
    glBegin(GL_TRIANGLES);
    for (auto polygon : data.polygons) {
        if (polygon.unique) {
            glColor3f(polygon.color(0), polygon.color(1), polygon.color(2));
        } else {
            glColor3f(data.overall(0), data.overall(1), data.overall(2));
        }
        normal3f(polygon.normal);
        vertex3f(polygon.r1);
        vertex3f(polygon.r2);
        vertex3f(polygon.r3);
    }
    glEnd();
}

}  // namespace Shape

#pragma pop

