#pragma once

// General modules
#include <string>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "blob.h"
#include "person.h"

namespace common {
    void drawPersonVec(cv::Mat& img_data, const std::vector<Person>& person_vec, const std::string type="ellipse");
    void drawBlobVec(cv::Mat& img_data, const std::vector<Blob>& blob_vec, const std::string type="ellipse");
    void drawBlobVecRect(cv::Mat& img_data, const std::vector<Blob>& blob_vec);
    void drawBlobVecEllipse(cv::Mat& img_data, const std::vector<Blob>& blob_vec);
    void drawBlob(cv::Mat& img_data, const struct data::rect rect);
    void drawBlob(cv::Mat& img_data, const struct data::ellipse ellipse);
    cv::Mat getROI(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy);
};
