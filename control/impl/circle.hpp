#pragma once 
#include <opencv/cv.hpp>

namespace circleSpace{
  std::pair<int,int> detectCircle(cv::Mat image, int show);
} //namespace circleSpace
