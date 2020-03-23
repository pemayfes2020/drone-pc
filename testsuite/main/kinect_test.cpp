#include "kinect.hpp"

#include <iostream>


int main()
{

    Kinect::start();

    while(true) {
        auto [rgb, ir, depth] = Kinect::getImages();
        cv::imshow("rgb", rgb);
        
        cv::waitKey(1);
    }
    

    return 0;
}
