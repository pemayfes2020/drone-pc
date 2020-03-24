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

    auto _depth = Control::Localization::depst{1.0, 0};

    while (true) {
        auto [rgb, ir, depth] = Kinect::getImages();
        cv::Mat depth_normalized, psuedo_color;
        depth.convertTo(depth_normalized, CV_8UC1, 65535.0f / 8000.0f * 255);
        cv::applyColorMap(depth_normalized, psuedo_color, cv::COLORMAP_AUTUMN);

        //cv::imshow("rgb", rgb);
        //cv::imshow("ir", ir);
        cv::imshow("psuedo colored depth", psuedo_color);

        auto [r, t] = Control::Localization::get2Dpos(rgb, depth, 0, _depth);
        if (_depth.rel) {
            std::cout << "ちゃんとdepth取れてるで自分" << std::endl;
        } else {
            std::cout << "depth = 0やで" << std::endl;
        }

        cv::waitKey(1);
    }

    return 0;
}
