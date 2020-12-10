#pragma once

// General modules
#include <vector>

// Local modules
#include "behav.h"
#include "blob_data.h"

class Person {
    private:
        int32_t m_id;
        bool m_enroll;

    public:
        // Members
        static uint32_t total_count;
        static uint32_t enroll_count;
        std::vector<BlobData> m_blob_data;

        // Constructors
        Person(const uint32_t frame_num, const Blob blob);

        // Getters
        int32_t getId(void) const;
        bool isEnrolled(void) const;

        // Setters
        void setId(const int32_t id);

        // General functions
        std::vector<Blob> getBlobVec(const uint32_t frame_num) const;
        std::vector<Blob> getBlobVecAll(void) const;
        uint32_t getLastFrame(void) const;
        bool isSplit(void);
        void changeId(const int32_t id);
        void enroll(void);
        void addBlobData(const uint32_t frame_num, const Blob blob);
};
