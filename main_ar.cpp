#include "message_types.hpp"
#include "unit.hpp"

#include <ardrone/ardrone.h>

#include <atomic>
#include <iostream>
#include <thread>

void init(ARDrone& ardrone)
{
    // Initialize
    if (!ardrone.open()) {
        std::cerr << "[Error] [Drone] Failed to initialize." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Info
    int major_version, minor_version, revision;
    ardrone.getVersion(&major_version, &minor_version, &revision);
    std::cout << "[Info] [Drone] Version: " << major_version << '.' << minor_version << '.' << revision << std::endl;
    std::cout << "[Info] [Drone] Battery: " << ardrone.getBatteryPercentage() << "[%]" << std::endl;
}

int main()
{
    ARDrone ardrone;
    init(ardrone);

    std::atomic_bool exit_flag = false;
    std::thread interprocess{[&exit_flag] { while(!exit_flag){} }};

    while (ardrone.update()) {
        Angular roll = ardrone.getRoll() * 1.0_rad;
        Angular pitch = ardrone.getPitch() * 1.0_rad;
        Angular yaw = ardrone.getYaw() * 1.0_rad;
        Length z = ardrone.getAltitude() * 1.0_m;
        double _vx, _vy, _vz;
        ardrone.getVelocity(&_vx, &_vy, &_vz);
        Vel vx = _vx * 1.0_m / 1.0_s,
            vy = _vx * 1.0_m / 1.0_s,
            vz = _vx * 1.0_m / 1.0_s;
    }

    ardrone.close();
    exit_flag = true;
    interprocess.join();

    return 0;
}
/*
class ARDrone
{
public:
    // Get an image
    virtual ARDRONE_IMAGE getImage(void);
    virtual ARDrone& operator>>(cv::Mat& image);
    virtual bool willGetNewImage(void);

    // Get AR.Drone's firmware version
    virtual int getVersion(int* major = NULL, int* minor = NULL, int* revision = NULL);

    // Get sensor values
    virtual double getRoll(void);                                                                          // Roll angle  [rad]
    virtual double getPitch(void);                                                                         // Pitch angle [rad]
    virtual double getYaw(void);                                                                           // Yaw angle   [rad]
    virtual double getAltitude(void);                                                                      // Altitude    [m]
    virtual double getVelocity(double* vx = NULL, double* vy = NULL, double* vz = NULL);                   // Velocity [m/s]
    virtual int getPosition(double* latitude = NULL, double* longitude = NULL, double* elevation = NULL);  // GPS (only for AR.Drone 2.0)

    // Battery charge [%]
    virtual int getBatteryPercentage(void);

    // Take off / Landing / Emergency
    virtual void takeoff(void);
    virtual void landing(void);
    virtual void emergency(void);

    // Move with velocity [m/s]
    virtual void move(double vx, double vy, double vr);
    virtual void move3D(double vx, double vy, double vz, double vr);

    // Change camera channel
    virtual void setCamera(int channel);

    // Animation
    virtual void setAnimation(int id, int duration = 0);              // Flight animation
    virtual void setLED(int id, float freq = 0.0, int duration = 0);  // LED animation

    // Calibration
    virtual void setFlatTrim(void);               // Flat trim
    virtual void setCalibration(int device = 0);  // Magnetometer calibration

    // Others
    virtual int onGround(void);                  // Check on ground
    virtual void setVideoRecord(bool activate);  // Video recording (only for AR.Drone 2.0)
    virtual void setOutdoorMode(bool activate);  // Outdoor mode (experimental)
};

*/
