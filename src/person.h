#pragma once

// General modules
#include <vector>
#include <tuple>

// Local modules
#include "blob.h"
#include "kalman.h"

typedef std::tuple<uint32_t, Blob, Kalman, std::vector<float>> BlobData;

class Person {
    private:
        std::vector<BlobData> m_data;
        bool m_target_init;

    public:
        static const uint8_t FRAME_NUM_INDEX = 0;
        static const uint8_t BLOB_INDEX = 1;
        static const uint8_t KALMAN_INDEX = 2;
        static const uint8_t TARGET_INDEX = 3;

        // Constructors
        Person();
        Person(uint32_t frame_num, Blob blob);

        // Getters
        std::vector<BlobData>* getData(void);
        std::vector<Blob> getBlobVec(void) const;
        uint32_t getLastFrame(void) const;
};
