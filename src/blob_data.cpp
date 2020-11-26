#include "blob_data.h"

// Constructors
BlobData::BlobData(const uint32_t frame_num, const Blob blob, const int32_t id) {
    m_frame_num = frame_num;
    m_target_model = std::vector<float>(behav::BINS, 0.0f);
    m_target_init = false;
    setId(id);
    m_blob = blob;
    m_kalman = Kalman();
}

// Getters
uint32_t BlobData::getFrameNum() const { return(m_frame_num); }
std::vector<float> BlobData::getTargetModel() const { return(m_target_model); }
bool BlobData::isTargetInit() const { return(m_target_init); }
int32_t BlobData::getId() const { return(m_id); }

// Setters
void BlobData::setFrameNum(const uint32_t frame_num) { m_frame_num = frame_num; }
void BlobData::setTargetModel(const std::vector<float> target_model) { m_target_model = target_model; }
void BlobData::setTargetInit() { m_target_init = true; }
void BlobData::setId(const int32_t id) { m_id = id; }
