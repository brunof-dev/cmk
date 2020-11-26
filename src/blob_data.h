#pragma once

// General modules
#include <vector>

// Local modules
#include "behav.h"
#include "blob.h"
#include "kalman.h"

class BlobData {
    private:
        uint32_t m_frame_num;
        std::vector<float> m_target_model;
        bool m_target_init;
        int32_t m_id;

    public:
        // Members
        Blob m_blob;
        Kalman m_kalman;

        // Constructors
        BlobData(const uint32_t frame_num, const Blob blob, const int32_t id);

        // Getters
        uint32_t getFrameNum(void) const;
        std::vector<float> getTargetModel(void) const;
        bool isTargetInit(void) const;
        int32_t getId(void) const;

        // Setters
        void setFrameNum(const uint32_t frame_num);
        void setTargetModel(const std::vector<float> target_model);
        void setTargetInit(void);
        void setId(const int32_t id);
};
