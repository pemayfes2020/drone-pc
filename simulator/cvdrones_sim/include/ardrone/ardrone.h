#pragma once

#include <opencv/cv.hpp>

// インターフェイスをあわせるため、.hにした

class ARDrone
{
public:
    // Constructor / Destructor
    ARDrone();
    ~ARDrone();

    // Initialize
    int open();

    // Update
    int update();

    // Finalize (Automatically called)
    void close();

    // Get an image
    cv::Mat getImage();
    ARDrone& operator>>(cv::Mat& image);
    bool willGetNewImage();

    // Get AR.Drone's firmware version
    int getVersion(int* major = nullptr, int* minor = nullptr, int* revision = nullptr);

    // Get sensor values
    double getRoll();                                                                                       // Roll angle  [rad]
    double getPitch();                                                                                      // Pitch angle [rad]
    double getYaw();                                                                                        // Yaw angle   [rad]
    double getAltitude();                                                                                   // Altitude    [m]
    double getVelocity(double* vx = nullptr, double* vy = nullptr, double* vz = nullptr);                   // Velocity [m/s]
    int getPosition(double* latitude = nullptr, double* longitude = nullptr, double* elevation = nullptr);  // GPS (only for AR.Drone 2.0)

    // Battery charge [%]
    int getBatteryPercentage();

    // Take off / Landing / Emergency
    void takeoff();
    void landing();
    void emergency();

    // Move with velocity [m/s]
    void move(double vx, double vy, double vr);
    void move3D(double vx, double vy, double vz, double vr);

    // Change camera channel
    void setCamera(int channel);

    // Animation
    void setAnimation(int id, int duration = 0);               // Flight animation
    void setLED(int id, float freq = 0.0f, int duration = 0);  // LED animation

    // Calibration
    void setFlatTrim();                   // Flat trim
    void setCalibration(int device = 0);  // Magnetometer calibration

    // Others
    int onGround();                      // Check on ground
    void setVideoRecord(bool activate);  // Video recording (only for AR.Drone 2.0)
    void setOutdoorMode(bool activate);  // Outdoor mode (experimental)
};
