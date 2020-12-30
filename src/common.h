#pragma once

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"

namespace common {
    bool sanity(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy);
    void fixCoord(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy);
    cv::Mat getROI(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy);
    bool readFrame(cv::VideoCapture& vid_reader, cv::Mat& img_data);
    template <typename T> void FIFO(const std::vector<T>& vec, std::vector<std::vector<T>>& vec_stack);
}

// Template
#include "common.tpp"
