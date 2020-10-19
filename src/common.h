#pragma once

// SDK modules
#include <opencv2/opencv.hpp>

namespace common {
    void draw(cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy);
    cv::Mat getROI(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy);
};
