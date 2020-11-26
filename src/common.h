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
    bool readFrame(cv::VideoCapture& vid_reader, cv::Mat& img_data);
    template <typename T> void FIFO(const std::vector<T>& vec, std::vector<std::vector<T>>& vec_stack);
    uint32_t distance(const struct data::rect a, const struct data::rect b);

    // Drawing functions
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

// Template
#include "common.tpp"
