#include "kinect.hpp"
#include "circle.hpp"

#include <opencv/cv.hpp>

#include <cstdlib>
#include <iostream>
#include <signal.h>


int main(int argc, char* argv[])
{

    Kinect::start();

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();

        //cv::imshow("rgb", rgb);
        //cv::imshow("ir", ir);
        cv::imshow("depth", depth);

        auto [x,y] = circleSpace::detectCircle(rgb, 1);

        std::cout << "(x,y) = (" << x << ", "<< y << ") " << std::endl; 
        cv::waitKey(1);
    }

    return 0;
}
