#pragma once

// General modules
#include <set>
#include <vector>

// SDK modules
#include <opencv2/opencv.hpp>

// Local modules
#include "behav.h"
#include "common.h"
#include "mean_shift.h"
#include "person.h"

namespace cmk {
    int32_t findNeighbour(const Blob blob, const std::vector<Blob>& blob_vec);
    void assignID(std::vector<Blob>& blob_vec, const std::vector<std::vector<Blob>>& blob_stack, const uint32_t frame_num, std::vector<Person>& person_vec);
    void split(Person& person, const std::vector<Person>& person_vec);
    void trackBlobDataVec(const cv::Mat& img_data, std::vector<BlobData>& blob_data_vec);
    void CMK(const cv::Mat& img_data, const uint32_t frame_num, std::vector<Person>& person_vec);
}
