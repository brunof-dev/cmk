#include "person.h"

// Static members
uint32_t Person::total_count = 0;
uint32_t Person::enroll_count = 0;

Person::Person(const uint32_t frame_num, const Blob blob) {
    total_count++;
    setId(total_count);
    setNbId(behav::INVALID_ID);
    m_enroll = false;
    BlobData blob_data(frame_num, blob, getId());
    m_blob_data.push_back(blob_data);
}

// Getters
int32_t Person::getId() const { return(m_id); }
int32_t Person::getNbId() const { return(m_nb_id); }
bool Person::isEnrolled(void) const { return(m_enroll); }

// Setters
void Person::setId(const int32_t id) { m_id = id; }
void Person::setNbId(const int32_t id) { m_nb_id = id; }

// General functions
std::vector<Blob> Person::getBlobVec(const uint32_t frame_num) const {
    std::vector<Blob> blob_vec;
    for (std::vector<BlobData>::const_iterator it = m_blob_data.cbegin(); it != m_blob_data.cend(); it++) {
        if (it->getFrameNum() == frame_num) blob_vec.push_back(it->m_blob);
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

uint32_t Person::getLastFrame(void) const { return (m_blob_data.at(m_blob_data.size() - 1).getFrameNum()); }

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
    BlobData blob_data(frame_num, blob, getId());
    m_blob_data.push_back(blob_data);
    if (!isEnrolled()) enroll();
}
