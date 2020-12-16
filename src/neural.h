#pragma once

// General modules
#include <vector>

// SDK modules
#include <ie_core.hpp>
#include <ie_plugin_config.hpp>
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "common.h"
#include "blob.h"

namespace neural {
    void setup(InferenceEngine::InferRequest& infer_req, uint16_t& net_width, uint16_t& net_height);
    void fillBlob(InferenceEngine::Blob::Ptr input_blob, const cv::Mat& img_data);
    void getBlobVec(InferenceEngine::Blob::Ptr output_blob, const uint16_t orig_width, const uint16_t orig_height, std::vector<Blob>& blob_vec);
    float IOU(const struct data::rect a, const struct data::rect b);
    bool isInner(const struct data::rect in, const struct data::rect out);
    void nonMaxSup(std::vector<Blob>& blob_vec);
    void detect(InferenceEngine::InferRequest& infer_req, const cv::Mat& img_data, const uint16_t net_width, const uint16_t net_height, std::vector<Blob>& blob_vec);
}
