#include "blob.h"

// Constructors
Blob::Blob() {
    m_dummy = true;
    setId(behav::INVALID_ID);
}

Blob::Blob(const struct data::rect rect) {
    m_dummy = false;
    setId(behav::INVALID_ID);
    updateAll(rect);
}

Blob::Blob(const struct data::ellipse ellipse) {
    m_dummy = false;
    setId(behav::INVALID_ID);
    updateAll(ellipse);
}

// Getters
struct data::rect Blob::getRect() const { return(m_rect); }
struct data::ellipse Blob::getEllipse() const { return(m_ellipse); }
bool Blob::isDummy(void) const { return(m_dummy); }
int32_t Blob::getId(void) const { return(m_id); }

// Setters
void Blob::setRect(const struct data::rect rect) { m_rect = rect; }
void Blob::setEllipse(const struct data::ellipse ellipse) { m_ellipse = ellipse; }
void Blob::setId(const int32_t id) { m_id = id; }

// Operators
bool Blob::operator==(const Blob& blob) const {
    bool rc = false;
    if ((this->m_rect == blob.m_rect) && (this->m_ellipse == blob.m_ellipse)) rc = true;
    return(rc);
}

bool Blob::operator!=(const Blob& blob) const {
    bool rc = false;
    if ((this->m_rect != blob.m_rect) || (this->m_ellipse == blob.m_ellipse)) rc = true;
    return(rc);
}

// General functions
struct data::rect Blob::getRectFromEllipse(const struct data::ellipse ellipse) {
    struct data::rect rect;
    // Save bounding box confidence
    rect.conf = m_rect.conf;
    // Calculate rectangle
    rect.xmin = static_cast<uint16_t>(ellipse.xc - ellipse.hx);
    rect.xmax = static_cast<uint16_t>(ellipse.xc + ellipse.hx);
    rect.ymin = static_cast<uint16_t>(ellipse.yc - ellipse.hy);
    rect.ymax = static_cast<uint16_t>(ellipse.yc + ellipse.hy);
    return(rect);
}

struct data::ellipse Blob::getEllipseFromRect(const struct data::rect rect) {
    struct data::ellipse ellipse;
    // Save ellipse similarity
    ellipse.simi = m_ellipse.simi;
    // Calculate ellipse
    ellipse.xc = static_cast<float>(rect.xmin) + 0.5f * static_cast<float>(rect.xmax - rect.xmin);
    ellipse.yc = static_cast<float>(rect.ymin) + 0.5f * static_cast<float>(rect.ymax - rect.ymin);
    ellipse.hx = 0.5f * static_cast<float>(rect.xmax - rect.xmin);
    ellipse.hy = 0.5f * static_cast<float>(rect.ymax - rect.ymin);
    return(ellipse);
}

void Blob::updateAll(const struct data::rect rect) {
    setRect(rect);
    struct data::ellipse ellipse = getEllipseFromRect(rect);
    setEllipse(ellipse);
}

void Blob::updateAll(const struct data::ellipse ellipse) {
    setEllipse(ellipse);
    struct data::rect rect = getRectFromEllipse(ellipse);
    setRect(rect);
}

void Blob::print(std::string type) const {
    if (!type.compare("rect")) {
        std::printf("xmin: %d\n", m_rect.xmin);
        std::printf("ymin: %d\n", m_rect.ymin);
        std::printf("xmax: %d\n", m_rect.xmax);
        std::printf("ymax: %d\n", m_rect.ymax);
    }
    else if (!type.compare("ellipse")) {
        std::printf("xc: %.2f\n", m_ellipse.xc);
        std::printf("yc: %.2f\n", m_ellipse.yc);
        std::printf("hx: %.2f\n", m_ellipse.hx);
        std::printf("hy: %.2f\n", m_ellipse.hy);
    }
}
