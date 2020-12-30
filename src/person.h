#pragma once
// General modules
#include <vector>

// Local modules
#include "behav.h"
#include "blob_data.h"

class Person {
    private:
        int32_t m_id;
        int32_t m_nb_id;
        bool m_enroll;
        bool m_lock;
        uint32_t m_lock_cnt;
        std::vector<Blob> m_lock_blob_vec;

    public:
        // Members
        static uint32_t total_count;
        static uint32_t enroll_count;
        std::vector<BlobData> m_blob_data;

        // Constructors
        Person(const uint32_t frame_num, const Blob blob);

        // Getters
        int32_t getId(void) const;
        int32_t getNbId(void) const;
        bool isEnrolled(void) const;
        bool isLocked(void) const;
        uint32_t getLockCnt(void) const;
        std::vector<Blob> getLockBlobVec(void) const;

        // Setters
        void setId(const int32_t id);
        void setNbId(const int32_t id);
        void lock(void);
        void unlock(void);
        void incrLockCnt(void);
        void setLockBlobVec(const std::vector<Blob> blob_vec);

        // General functions
        std::vector<Blob> getBlobVec(const uint32_t frame_num) const;
        uint32_t getLastFrame(void) const;
        std::vector<Blob> getLastBlobVec(void) const;
        std::vector<Blob> getBlobVecAll(void) const;
        bool isSplit(void);
        void changeId(const int32_t id);
        void enroll(void);
        void addBlobData(const uint32_t frame_num, const Blob blob);
        void addBlobData(const uint32_t frame_num, const std::vector<Blob> blob_vec);
        void removeLastBlobData(void);
};
