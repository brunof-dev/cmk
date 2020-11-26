#include "person.h"

// Static members
uint32_t Person::count = 0;

Person::Person(const uint32_t frame_num, const Blob blob) {
    count++;
    setId(count);
    BlobData blob_data(frame_num, blob, getId());
    m_blob_data.push_back(blob_data);
}

// Getters
int32_t Person::getId() const { return(m_id); }

// Setters
void Person::setId(const int32_t id) { m_id = id; }

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

void Person::addBlobData(const uint32_t frame_num, const Blob blob) {
    BlobData blob_data(frame_num, blob, getId());
    m_blob_data.push_back(blob_data);
}

bool Person::isSplit() { return(getBlobVec(getLastFrame()).size() > 1); }
