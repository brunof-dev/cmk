#pragma once

// General modules
#include <string>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "blob.h"
#include "person.h"

namespace common {
    // General functions
    void getFrameDim(const cv::Mat& img_data, uint16_t& frame_width, uint16_t& frame_height);
    bool sanity(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy);
    void fixCoord(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy);
    cv::Mat getROI(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy);

    // Drawing functions
    void drawPersonVec(cv::Mat& img_data, const uint32_t frame_num, const std::vector<Person>& person_vec, const std::string type="ellipse");
    void drawBlobVec(cv::Mat& img_data, const std::vector<Blob>& blob_vec, const std::string type="ellipse");
    void drawBlobVecRect(cv::Mat& img_data, const std::vector<Blob>& blob_vec);
    void drawBlobVecEllipse(cv::Mat& img_data, const std::vector<Blob>& blob_vec);
    void drawBlob(cv::Mat& img_data, const struct data::rect rect);
    void drawBlob(cv::Mat& img_data, const struct data::ellipse ellipse);
};
