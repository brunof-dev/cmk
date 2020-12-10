#pragma once

// General modules
#include <vector>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "common.h"
#include "mean_shift.h"
#include "person.h"

namespace cmk {
    void findNeighbour(const Blob blob, const std::vector<Blob>& blob_vec, int32_t& nb_index);
    void assignID(std::vector<Blob>& blob_vec, const std::vector<std::vector<Blob>>& blob_stack, const uint32_t frame_num, std::vector<Person>& person_vec);
    void track(const cv::Mat& img_data, std::vector<BlobData>& blob_data_vec);
}
