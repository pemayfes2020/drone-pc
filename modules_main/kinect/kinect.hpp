#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include <libfreenect2/packet_pipeline.h>

#include <opencv/cv.hpp>

namespace Kinect
{

struct Images {
    cv::Mat rgb;
    cv::Mat ir;
    cv::Mat depth;
};

void start();
Images getImages();

}  // namespace Kinect
