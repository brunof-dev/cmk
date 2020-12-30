#pragma once

// General modules
#include <string>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "person.h"

namespace draw {
    void drawPersonVec(cv::Mat& img_data, const uint32_t frame_num, const std::vector<Person>& person_vec, const std::string type="ellipse");
    cv::Scalar getRandColor(const uint32_t seed);
    void drawBlobVec(cv::Mat& img_data, const uint32_t id, const std::vector<Blob>& blob_vec, const std::string type);
    void drawVecRect(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec);
    void drawVecEllipse(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec);
    void drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::rect rect);
    void drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::ellipse ellipse);
    void drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::rect rect);
    void drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::ellipse ellipse);
};
