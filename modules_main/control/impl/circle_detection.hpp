#pragma once

#include "params/recognition_params.hpp"

#include <Eigen/Core>
#include <opencv/cv.hpp>

#include <vector>

namespace StarReckoning
{

inline Eigen::Vector2i detectCircle(cv::Mat image, bool show_image = true)
{
    using namespace Params::CircleDetection;

    cv::Mat saturation, value, hue_saturation, image_black_white;

    std::vector<std::pair<cv::Point, double>> circles;

    // Lowpass filter
    cv::Mat image_filtered;
    cv::GaussianBlur(image, image_filtered, cv::Size(9, 9), 2, 2);

    // HSV空間に変換する
    cv::Mat hsv;
    cv::cvtColor(image_filtered, hsv, CV_BGR2HSV);

    // hsvを各チャンネルに分解する([0]:h, [1]:s,[2]:v)
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);

    // 2値化 {

    // lowerlimit < hue < upperlimit
    cv::Mat hue_highpass, hue_lowpass, hue_bandpass;
    cv::threshold(hsv_channels[0], hue_highpass, HUE_LOWER_LIMIT, 255, CV_THRESH_BINARY);
    cv::threshold(hsv_channels[0], hue_lowpass, HUE_UPPER_LIMIT, 255, CV_THRESH_BINARY_INV);
    cv::bitwise_and(hue_lowpass, hue_highpass, hue_bandpass);

    // lowerlimit < saturation
    cv::Mat saturation_highpass;
    cv::threshold(hsv_channels[1], saturation, SATURATION_LOWER_LIMIT, 255, CV_THRESH_BINARY);  //彩度LOW_SATURATION以上

    // lowerlimit < value
    cv::Mat value_highpass;
    cv::threshold(hsv_channels[2], value, VALUE_LOWER_LIMIT, 255, CV_THRESH_BINARY);  //明度LOW_VALUE以上

    // 結合bandpass filter
    cv::Mat value_saturation_filtered, image_binarized;
    cv::bitwise_and(saturation_highpass, value_highpass, value_saturation_filtered);
    cv::bitwise_and(value_saturation_filtered, hue_bandpass, image_binarized);

    // }


    // 領域探索
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(image_binarized, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    int contours_num = contours.size();
    struct Contour {
        std::vector<cv::Point> polygon;
        cv::Rect bound_rect;
        cv::Point2f center;
        float radius;
    };
    std::vector<Contour> summaries;

    // 円形度最大の領域を探索する
    double circularity_max = 0.0;
    int circularity_max_id = 0;

    for (int i = 0; i < contours.size(); ++i) {
        cv::approxPolyDP(cv::Mat(contours[i]), summaries[i].polygon, 3, true);
        summaries[i].bound_rect = cv::boundingRect(cv::Mat{summaries[i].polygon});
        cv::minEnclosingCircle(cv::Mat{summaries[i].polygon}, summaries[i].center, summaries[i].radius);

        double arc_length = cv::arcLength(contours[i], true);
        double area_size = cv::contourArea(contours[i]);
        double circularity = 4 * M_PI * area_size / (arc_length * arc_length);  // 円のとき最大値1を取る

        if (circularity > circularity_max) {
            circularity_max = circularity;
            circularity_max_id = i;
        }
    }

    if (circularity_max > CIRCULARITY_THRESHOLD) {
        if (show_image) {
            cv::circle(image, summaries[circularity_max_id].center, (int)summaries[circularity_max_id].radius, cv::Scalar(0, 0, 255), 2, 8, 0);
            cv::imshow("kinect_image", image);
        }
        return Eigen::Vector2i{summaries[circularity_max_id].center.x, summaries[circularity_max_id].center.y};
    } else {
        return Eigen::Vector2i::Zero();
    }
}

}  // namespace StarReckoning
