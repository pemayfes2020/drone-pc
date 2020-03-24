#include "circle.hpp"
#include <iostream>
#include <vector>

constexpr int LOW_SATURATION = 65;  //saturation（彩度）の下限
constexpr int LOW_VALUE = 50;       //value（明度）の下限

// parameters for the blue ball
constexpr int LOW_HUE = 95;
constexpr int UP_HUE = 105;


namespace circleSpace
{
std::pair<int, int> detectCircle(cv::Mat image, int show)
{
    cv::Mat hsv, frame, hue, hue1, hue2, saturation, value, hue_saturation, image_black_white;

    std::vector<std::pair<cv::Point, double>> circles;

    cv::cvtColor(image, hsv, CV_BGR2HSV);
    cv::Mat img = image.clone();
    // Lowpass filter
    cv::GaussianBlur(img, img, cv::Size(9, 9), 2, 2);
    //cv::bilateralFilter(img, img, 10, 100, 10, cv::BORDER_DEFAULT);

    std::vector<cv::Mat> singlechannels;  //Matクラスのベクトルとしてsinglechannelsを定義

    cv::split(hsv, singlechannels);  //hsvをsinglechannelsに分解([0]:h, [1]:s,[2]:v)

    // それぞれのチャンネルことに閾値を設定して二値化
    // singlechannels[0]をLOW_HUEを閾値処理して、LOW_HUE以上の部分が255,それ以下の部分が0になるように、hue1に格納する。
    cv::threshold(singlechannels[0], hue1, LOW_HUE, 255, CV_THRESH_BINARY);
    // singlechannels[0]をUP_HUEを閾値処理して、UP_HUE以上の部分が0,それ以下の部分が255になるように、hue2に格納する。
    cv::threshold(singlechannels[0], hue2, UP_HUE, 255, CV_THRESH_BINARY_INV);
    cv::threshold(singlechannels[1], saturation, LOW_SATURATION, 255, CV_THRESH_BINARY);  //彩度LOW_SATURATION以上
    cv::threshold(singlechannels[2], value, LOW_VALUE, 255, CV_THRESH_BINARY);            //明度LOW_VALUE以上
    //条件を満たした領域をoutに設定
    cv::bitwise_and(hue1, hue2, hue);
    cv::bitwise_and(hue, saturation, hue_saturation);           // hueとsaturationのbitごとのandをとる→hue_saturation
    cv::bitwise_and(hue_saturation, value, image_black_white);  // hue_saturationとvalueのbitごとのandをとる→image_black_white

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(image_black_white, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> contours_poly(contours.size());
    std::vector<cv::Rect> bound_rect(contours.size());
    std::vector<cv::Point2f> center(contours.size());
    std::vector<float> radius(contours.size());

    double cand_circle_label = 0.0;
    int circle_ind = 0;

    for (int i = 0; i < contours.size(); i++) {
        cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
        bound_rect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
        cv::minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
        double L = cv::arcLength(contours[i], true);
        double S = cv::contourArea(contours[i]);
        double circle_label = 4 * M_PI * S / (L * L);


        /*if(circle_label > 0.47  && radius[i] > 8.0){
                circle(image, center[i], (int)radius[i], cv::Scalar(0, 0, 255), 2, 8, 0 );
                std::cout << "center = (" << center[i].x << ' ' << center[i].y << ") r = " << radius[i] << std::endl;
                circles.push_back(std::make_pair(cv::Point(cvRound(center[i].x),cvRound(center[i].y)),radius[i]));
                //cout << "radius = " << radius[i] << " S = " << S << endl;
            }*/
        if (circle_label > cand_circle_label) {
            cand_circle_label = circle_label;
            circle_ind = i;
        }
    }

    if (center.size()) {
        if (show) {
            circle(image, center[circle_ind], (int)radius[circle_ind], cv::Scalar(0, 0, 255), 2, 8, 0);
            cv::imshow("kineImg", image);
        }
        return {center[circle_ind].x, center[circle_ind].y};
    } else {
        return {0, 0};
    }
}
}  //namespace circleSpace
