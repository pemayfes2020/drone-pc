#define GL_SILENCE_DEPRECATION


#include "graphic/graphic.hpp"

#include <GL/glut.h>

#include <Eigen/Core>

#include <cmath>
#include <iostream>

namespace Graphic
{

using namespace Eigen;

constexpr float PI = 3.1415926535;

static std::vector<Object> objects;
static void (*user_callback)(std::vector<Object>&);

static int window_height = 600;
static int window_width = 600;

static float cam_dist = 4000.0;
static float cam_theta = PI / 4.0;
static float cam_phi = PI / 5.0;

Vector3f cam;
Vector3f cam_top;

static Color bg_color{0.0, 0.0, 0.0};


void setWindowSize(int height, int width)
{
    window_height = height;
    window_width = width;
    glutReshapeWindow(width, height);
}

void setBGColor(Color color)
{
    bg_color = color;
}

void updateCameraPos()
{
    cam = Vector3f{
        cam_dist * std::cos(cam_theta) * std::cos(cam_phi),
        cam_dist * std::sin(cam_theta) * std::cos(cam_phi),
        cam_dist * std::sin(cam_phi)};

    float norm = cam.norm();
    float s = cam(2) / (cam(2) - norm * norm);

    cam_top = s * cam + (1.0 - s) * Vector3f{0.0, 0.0, 1.0};
}

void drawCoordinate(int measure, float size)
{
    const auto drawLine = [](const Vector3f& from, const Vector3f& to) {
        //線幅
        glLineWidth(1.0);
        //線
        glBegin(GL_LINES);
        glVertex3f(from(0), from(1), from(2));
        glVertex3f(to(0), to(1), to(2));
        glEnd();
    };

    glDisable(GL_LIGHTING);
    // xy平面
    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    for (int x = 0; x <= measure; x++) {
        drawLine(
            {x * size - (size * measure / 2), -(size * measure / 2), 0},
            {x * size - (size * measure / 2), measure * size - (size * measure / 2), 0});
    }
    for (int y = 0; y <= measure; y++) {
        drawLine(
            {-(size * measure / 2), y * size - (size * measure / 2), 0},
            {measure * size - (size * measure / 2), y * size - (size * measure / 2), 0});
    }

    glDisable(GL_DEPTH_TEST);
    Vector3f origin{0, 0, 0};
    // x(Red)
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    drawLine(origin, {(measure / 2 + 2) * size, 0, 0});
    // y(Green)
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    drawLine(origin, {0, (measure / 2 + 2) * size, 0});
    // z(Blue)
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    drawLine(origin, {0, 0, (measure / 2 + 2) * size});

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

static void display(void)
{
    glClearColor(bg_color(0), bg_color(1), bg_color(2), 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (float)window_width / window_height, 1.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // 行列を初期化
    gluLookAt(cam(0), cam(1), cam(2), 0.0, 0.0, 0.0, cam_top(0), cam_top(1), cam_top(2));
    glPopMatrix();

    drawCoordinate(20, 1000);

    for (auto& obj : objects) {
        glColor3f(obj.color(0), obj.color(1), obj.color(2));
        obj.shape->start_draw();
        obj.shape->draw();
        obj.shape->end_draw();
    }

    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    window_width = w;
    window_height = h;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120.0, (float)window_width / window_height, 1.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // 行列を初期化

    gluLookAt(cam(0), cam(1), cam(2), 0.0, 0.0, 0.0, cam_top(0), cam_top(1), cam_top(2));
}

static void idle()
{
    user_callback(objects);
    glutPostRedisplay();
}

namespace Mouse
{

bool left_button_down = false;
bool right_button_down = false;
bool h_down = false;
Vector2d pos{0, 0};
float theta_ratio = PI * 3.0;
float phi_ratio = PI * 1.0;
float dist_ratio = 1000.0;

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            left_button_down = true;
            pos = Vector2d{x, y};
        } else if (state == GLUT_UP) {
            left_button_down = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            right_button_down = true;
            pos = Vector2d{x, y};
        } else if (state == GLUT_UP) {
            right_button_down = false;
        }
        break;
    }
}

static void motion(int x, int y)
{
    Vector2d pos_new = Vector2d{x, y};

    if (!left_button_down && !right_button_down) {
        pos = pos_new;
        return;
    }

    if (right_button_down) {
        // zoom mode
        cam_dist += (pos_new(1) - pos(1)) / window_height * dist_ratio;
    } else {
        // rotate mode
        cam_theta -= (pos_new(0) - pos(0)) / window_width * theta_ratio;
        cam_phi += (pos_new(1) - pos(1)) / window_height * phi_ratio;
    }
    pos = pos_new;

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120.0, (float)window_width / window_height, 1.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // 行列を初期化

    updateCameraPos();

    gluLookAt(cam(0), cam(1), cam(2), 0.0, 0.0, 0.0, cam_top(0), cam_top(1), cam_top(2));

    glPopMatrix();

    glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
    if (key == 'h') {
        cam_theta = PI / 4.0;
        cam_phi = PI / 5.0;
        updateCameraPos();

        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(120.0, (float)window_width / window_height, 1.0, 10000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();  // 行列を初期化


        gluLookAt(cam(0), cam(1), cam(2), 0.0, 0.0, 0.0, cam_top(0), cam_top(1), cam_top(2));

        glPopMatrix();

        glutPostRedisplay();
    }
}

}  // namespace Mouse


void init(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(Mouse::mouse);
    glutMotionFunc(Mouse::motion);
    glutKeyboardFunc(Mouse::key);

    updateCameraPos();

    // 3D settings
    GLfloat light_position[] = {20.0, 200.0, 800.0, 0.0};

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void start(void (*callback)(std::vector<Object>&))
{
    user_callback = callback;
    glutMainLoop();
}


Object& addSphere(const Vector3f& position, const Vector3f& rotation, float radius, Color color)
{
    objects.push_back(
        Object{color, std::make_shared<Shape::Sphere>(position, rotation, radius)});
    return objects.back();
}

Object& addPlane(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float height, float width, Color color)
{
    objects.push_back(
        Object{color, std::make_shared<Shape::Plane>(position, rotation, height, width)});
    return objects.back();
}


Object& addRectangular(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, const Eigen::Vector3f& size, Color color)
{
    objects.push_back(
        Object{color, std::make_shared<Shape::Rectangular>(position, rotation, size)});
    return objects.back();
}

Object& addCylinder(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation, float radius, float height, Color color)
{

    objects.push_back(
        Object{color, std::make_shared<Shape::Cylinder>(position, rotation, radius, height)});
    return objects.back();
}

Object& addTeapot(const Vector3f& position, const Vector3f& rotation, int size, Color color)
{
    objects.push_back(
        Object{color, std::make_shared<Shape::Teapot>(position, rotation, size)});
    return objects.back();
}

Object& addSTLModel(const Vector3f& position, const Vector3f& rotation, const std::string filepath, bool colored, Color color)
{
    if (colored) {
        objects.push_back(
            Object{color, std::make_shared<Shape::ColoredSTLModel>(position, rotation, filepath)});
    } else {
        objects.push_back(
            Object{color, std::make_shared<Shape::STLModel>(position, rotation, filepath)});
    }
    return objects.back();
}

}  // namespace Graphic
