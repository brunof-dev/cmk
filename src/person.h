#pragma once

// General modules
#include <vector>

// Local modules
#include "blob_data.h"

class Person {
    private:
        int32_t m_id;

    public:
        // Members
        static uint32_t count;
        std::vector<BlobData> m_blob_data;

        // Constructors
        Person(const uint32_t frame_num, const Blob blob);

        // Getters
        int32_t getId(void) const;

        // Setters
        void setId(const int32_t id);

        // General functions
        std::vector<Blob> getBlobVec(const uint32_t frame_num) const;
        std::vector<Blob> getBlobVecAll(void) const;
        uint32_t getLastFrame(void) const;
        bool isSplit(void);
        void addBlobData(const uint32_t frame_num, const Blob blob);
};
