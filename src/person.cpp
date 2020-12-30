#include "person.h"
#include <iostream>

// Static members
uint32_t Person::total_count = 0;
uint32_t Person::enroll_count = 0;

Person::Person(const uint32_t frame_num, const Blob blob) {
    total_count++;
    setId(total_count);
    setNbId(behav::INVALID_ID);
    m_enroll = false;
    m_lock = false;
    m_lock_cnt = 0;
    BlobData blob_data(frame_num, blob, getId());
    m_blob_data.push_back(blob_data);
}

// Getters
int32_t Person::getId() const { return(m_id); }
int32_t Person::getNbId() const { return(m_nb_id); }
bool Person::isEnrolled() const { return(m_enroll); }
bool Person::isLocked() const { return(m_lock); }
uint32_t Person::getLockCnt() const { return(m_lock_cnt); }
std::vector<Blob> Person::getLockBlobVec() const { return(m_lock_blob_vec); }

// Setters
void Person::setId(const int32_t id) { m_id = id; }
void Person::setNbId(const int32_t id) { m_nb_id = id; }
void Person::lock() { m_lock = true; }
void Person::unlock() {
    m_lock = false;
    m_lock_cnt = 0;
}
void Person::incrLockCnt() { m_lock_cnt++; }
void Person::setLockBlobVec(const std::vector<Blob> blob_vec) { m_lock_blob_vec = blob_vec; }

// General functions
std::vector<Blob> Person::getBlobVec(const uint32_t frame_num) const {
    std::vector<Blob> blob_vec;
    for (std::vector<BlobData>::const_iterator it = m_blob_data.cbegin(); it != m_blob_data.cend(); it++) {
        if (it->getFrameNum() == frame_num) blob_vec.push_back(it->m_blob);
    }
    return(blob_vec);
}

uint32_t Person::getLastFrame() const { return (m_blob_data.at(m_blob_data.size() - 1).getFrameNum()); }

std::vector<Blob> Person::getLastBlobVec() const {
    std::vector<Blob> blob_vec;
    uint32_t last_frame = getLastFrame();
    for (std::vector<BlobData>::const_iterator it = m_blob_data.cbegin(); it != m_blob_data.cend(); it++) {
        if (it->getFrameNum() == last_frame) {
            blob_vec.push_back(it->m_blob);
        }
    }
    return(blob_vec);
}

std::vector<Blob> Person::getBlobVecAll() const {
    std::vector<Blob> blob_vec;
    for (std::vector<BlobData>::const_iterator it = m_blob_data.cbegin(); it != m_blob_data.cend(); it++) {
        blob_vec.push_back(it->m_blob);
    }
    return(blob_vec);
}

bool Person::isSplit() { return(getBlobVec(getLastFrame()).size() > 1); }

void Person::changeId(const int32_t id) {
    setId(id);
    for (std::vector<BlobData>::iterator it = m_blob_data.begin(); it != m_blob_data.end(); it++) {
        it->setId(id);
        it->m_blob.setId(id);
    }
}

void Person::enroll() {
    if (m_blob_data.size() >= behav::OCURRENCE) {
        setNbId(m_id);
        m_enroll = true;
        enroll_count++;
        changeId(enroll_count);
    }
}

void Person::addBlobData(const uint32_t frame_num, const Blob blob) {
    if (!isLocked()) {
        BlobData blob_data(frame_num, blob, getId());
        m_blob_data.push_back(blob_data);
        if (!isEnrolled()) enroll();
    }
}

void Person::addBlobData(const uint32_t frame_num, const std::vector<Blob> blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        addBlobData(frame_num, *it);
    }
}

void Person::removeLastBlobData() { m_blob_data.pop_back(); }
