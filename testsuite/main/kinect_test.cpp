#include "kinect/kinect.hpp"

#include "safe_exit.hpp"
#include <iostream>


int main()
{
    ThreadRoom::setExitHandler();
    Kinect::start();

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();
        cv::imshow("rgb", rgb);

        cv::waitKey(1);
    }


    return 0;
}
