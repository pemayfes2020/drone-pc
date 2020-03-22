#define GL_SILENCE_DEPRECATION

#include "shape.hpp"

#include <GL/glut.h>

#include <Eigen/Core>

#include <iostream>

namespace Shape
{

using namespace Eigen;

static double rad2deg(double rad)
{
    return rad / 3.14159265358979 * 180.0;
}
void ShapeBase::start_draw()
{
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glRotatef(rad2deg(rotation(0)), 1.0, 0.0, 0.0);
    glRotatef(rad2deg(rotation(1)), 0.0, 1.0, 0.0);
    glRotatef(rad2deg(rotation(2)), 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glTranslatef(position(0), position(1), position(2));
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

}  // namespace Shape
