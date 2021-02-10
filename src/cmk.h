#pragma once

// General modules
#include <set>
#include <vector>
#include <cstdio>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "common.h"
#include "frame.h"
#include "geometry.h"
#include "mean_shift.h"
#include "person.h"

namespace cmk {
    int32_t findNeighbour(const Blob blob, const std::vector<Blob>& blob_vec);
    void assignID(std::vector<Blob>& blob_vec, const std::vector<std::vector<Blob>>& blob_stack, const uint32_t frame_num, std::vector<Person>& person_vec);
    std::vector<Blob> getNewBlobVec(const struct data::rect orig_rect);
    void trackBlobDataVec(const cv::Mat& img_data, const uint32_t frame_num, std::vector<BlobData>& blob_data_vec);
    void split(const uint32_t frame_num, Person& person, std::vector<Person>& person_vec);
    void collapse(const uint32_t frame_num, Person& person);
    void CMK(const cv::Mat& img_data, const uint32_t frame_num, std::vector<Person>& person_vec);
}
