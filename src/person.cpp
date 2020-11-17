#include "person.h"

// Constructor
Person::Person() {
}

Person::Person(uint32_t frame_num, Blob blob) {
    BlobData tup(frame_num, blob, Kalman(), std::vector<float>(behav::BINS, 0.0f));
    m_data.push_back(tup);
}

// Getters
std::vector<BlobData>* Person::getData() { return(&m_data); }

std::vector<Blob> Person::getBlobVec() const {
    std::vector<Blob> blob_vec;
    for (std::vector<BlobData>::const_iterator it = m_data.cbegin(); it != m_data.cend(); it++) {
        blob_vec.push_back(std::get<BLOB_INDEX>(*it));
    }
    return(blob_vec);
}

uint32_t Person::getLastFrame(void) const { return (std::get<FRAME_NUM_INDEX>(m_data.at(m_data.size() - 1))); }
