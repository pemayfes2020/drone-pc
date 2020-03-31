#include "graphic/graphic.hpp"

#include <Eigen/Core>

#include <cmath>
#include <iostream>
Eigen::Vector3f pos;
Eigen::Vector3f rot;

namespace drone
{

Graphic::Object body;
Graphic::Object ball;
float ball_offset = 80.0;
}  // namespace drone

namespace project_room
{
struct interior {
    Graphic::Object obj;
    Eigen::Vector3f pos;
    Eigen::Vector3f rot;
};

float wall_height = 4000.0f;
float wall_width = 6000.0f;
}  // namespace project_room

float t;
int plusminus = 1.0;
float speed = 15.0;

constexpr float PI = 3.1415926535;
void callback(std::vector<Graphic::Object>& objs)
{
    if (t >= project_room::wall_width / 2. - 300.) {
        plusminus = -1.0;
    } else if (t <= -project_room::wall_width / 2. + 300.) {
        plusminus = 1.0;
    }
    std::cout << t << std::endl;
    t += speed * plusminus;
    pos << t, 0.0f, 0.0f;
    drone::body.shape->position = pos;
    drone::body.shape->rotation = rot;
    drone::ball.shape->position = Eigen::Vector3f{pos(0), pos(1), pos(2) + drone::ball_offset};
    drone::ball.shape->rotation = rot;
}

int main(int argc, char** argv)
{
    Graphic::init(argc, argv);
    t = 0;
    Graphic::setWindowSize(800, 800);
    Graphic::setBGColor(Color{0.2, 0.2, 0.2});

    pos << 0.0f, 0.0f, 0.0f;
    rot << 0.0f, 0.0f, PI / 2.;

    drone::body = Graphic::addSTLModel(pos, rot, "../resource/ardrone.stl", Color{1.0, 1.0, 0.0});
    drone::ball = Graphic::addSphere(Eigen::Vector3f{pos(0), pos(1), pos(2) + drone::ball_offset}, rot, 40, Color{0.0, 0.2, 0.8});


    project_room::interior Wall[4];

    //床
    Wall[0].pos << 0.0f, 0.0f, 0.0f;
    Wall[0].rot << 0.0f, 0.0f, 0.0f;
    Wall[0].obj = Graphic::addPlane(Wall[0].pos, Wall[0].rot, project_room::wall_width, project_room::wall_width, Color{0.9, 0.9, 0.9});
    //床


    Wall[1].pos << 0.0f, -project_room::wall_width / 2., 0.0f;
    Wall[1].rot << PI / 2., 0.0f, 0.0f;
    Wall[1].obj = Graphic::addPlane(Wall[1].pos, Wall[1].rot, project_room::wall_width, project_room::wall_height, Color{0.9, 0.9, 0.9});

    Wall[2].pos << -project_room::wall_width / 2., 0.0f, 0.0f;
    Wall[2].rot << 0.0f, PI / 2., 0.0f;
    Wall[2].obj = Graphic::addPlane(Wall[2].pos, Wall[2].rot, project_room::wall_height, project_room::wall_width, Color{0.9, 0.9, 0.9});


    Graphic::start(callback);

    return 0;
}
