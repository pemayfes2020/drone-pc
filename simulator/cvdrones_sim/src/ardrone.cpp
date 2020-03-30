#include "core/core.hpp"

#include "ardrone/ardrone.h"

#include <cmath>
#include <iostream>
#include <string>

ARDrone::ARDrone()
{
}

ARDrone::~ARDrone()
{
}


int ARDrone::open()
{
    return 0;
}

int ARDrone::update()
{
    return 0;
}

void ARDrone::close()
{
}

cv::Mat ARDrone::getImage()
{
    return cv::Mat{};
}

ARDrone& ARDrone::operator>>(cv::Mat& image)
{
}

bool ARDrone::willGetNewImage()
{
    return 0;
}

int ARDrone::getVersion(int* major, int* minor, int* revision)
{
    return 0;
}


double ARDrone::getRoll()
{
}
double ARDrone::getPitch()
{
}
double ARDrone::getYaw()
{
    return 0.0;
}
double ARDrone::getAltitude()
{
    return 0.0;
}
double ARDrone::getVelocity(double* vx, double* vy, double* vz)
{
    return 0.0;
}
int ARDrone::getPosition(double* latitude, double* longitude, double* elevation)
{
    return 0;
}


int ARDrone::getBatteryPercentage()
{
    return 0;
}


void ARDrone::takeoff()
{
}
void ARDrone::landing()
{
}
void ARDrone::emergency()
{
}


void ARDrone::move(double vx, double vy, double vr)
{
}
void ARDrone::move3D(double vx, double vy, double vz, double vr)
{
}


void ARDrone::setCamera(int channel)
{
}


void ARDrone::setAnimation(int id, int duration)
{
}
void ARDrone::setLED(int id, float freq, int duration)
{
}


void ARDrone::ARDrone::setFlatTrim() {}
void ARDrone::setCalibration(int device) {}


int ARDrone::onGround()
{
    return 0;
}

void ARDrone::setVideoRecord(bool activate) {}

void ARDrone::setOutdoorMode(bool activate) {}

