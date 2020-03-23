#include "circle.hpp"
#include "kinect.hpp"
#include "localization.hpp"
#include "safe_exit.hpp"

#include <opencv/cv.hpp>

#include <cstdlib>
#include <iostream>
#include <signal.h>


int main(int argc, char* argv[])
{
    ThreadRoom::setExitHandler();

    Kinect::start();

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();

        //cv::imshow("rgb", rgb);
        //cv::imshow("ir", ir);
        cv::imshow("depth", depth);

        auto [r, t] = Control::Localization::get2Dpos(rgb, depth, 0);

        cv::waitKey(1);
    }

    return 0;
}
