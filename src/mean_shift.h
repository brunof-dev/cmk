#pragma once

// General modules
#include <iostream>
#include <cmath>
#include <string>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "common.h"

namespace mean_shift {
    float getSqDist(const float x, const float y);
    float getKE(const float x);
    float getNormFactor(const float hx, const float hy);
    uint16_t getBin(const uint8_t blue, const uint8_t green, const uint8_t red);
    std::vector<float> getModel(const cv::Mat& roi_data, const float xc, const float yc, const float hx, const float hy,
                                const float norm_factor);
    float getSimi(const std::vector<float>& a, const std::vector<float>& b);
    float getWeight(const std::vector <float>& target_model, const std::vector <float>& cand_model, const uint16_t bin);
    void getShiftVector(const cv::Mat& roi_data, const float xc, const float yc, const float hx, const float hy,
                        const std::vector <float>& target_model, const std::vector <float>& cand_model,
                        float& delta_x, float& delta_y);
    float updateCenter(const cv::Mat& img_data, float& xc, float& yc, const float hx, const float hy,
                       const std::vector<float>& target_model);
    float meanShift(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy,
                    const std::vector<float>& target_model);
}
