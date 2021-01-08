#include "geometry.h"

uint32_t geometry::distance(const struct data::rect a, const struct data::rect b) {
    // Coordinates of the center of the bounding boxes
    uint16_t a_xc = static_cast<uint16_t>((a.xmin + a.xmax) * 0.5);
    uint16_t a_yc = static_cast<uint16_t>((a.ymin + a.ymax) * 0.5);
    uint16_t b_xc = static_cast<uint16_t>((b.xmin + b.xmax) * 0.5);
    uint16_t b_yc = static_cast<uint16_t>((b.ymin + b.ymax) * 0.5);

    // Distance
    uint32_t dist = (a_xc - b_xc) * (a_xc - b_xc) + (a_yc - b_yc) * (a_yc - b_yc);
    return(dist);
}

float geometry::IOU(const struct data::rect a, const struct data::rect b) {
    // Assert correctness
    assert(a.xmin < a.xmax);
    assert(a.ymin < a.ymax);
    assert(b.xmin < b.xmax);
    assert(b.ymin < b.ymax);

    // Coordinates of intersection rectangle
    const uint16_t x_left = std::max(a.xmin, b.xmin);
    const uint16_t x_right = std::min(a.xmax, b.xmax);
    const uint16_t y_top = std::max(a.ymin, b.ymin);
    const uint16_t y_bottom = std::min(a.ymax, b.ymax);

    // Case of no intersection
    float iou;
    if ((x_right < x_left) || (y_bottom < y_top)) {
        iou = 0.0;
    }
    else {
        // Calculate areas and ratio
        const uint32_t i_area = (x_right - x_left + 1) * (y_bottom - y_top + 1);
        const uint32_t a_area = (a.xmax - a.xmin + 1) * (a.ymax - a.ymin + 1);
        const uint32_t b_area = (b.xmax - b.xmin + 1) * (b.ymax - b.ymin + 1);
        iou = static_cast<float>(i_area) / static_cast<float>(a_area + b_area - i_area);
    }
    return(iou);
}

bool geometry::isInner(const struct data::rect in, const struct data::rect out) {
    bool rc = false;
    if ((in.xmin > out.xmin) && (in.xmax < out.xmax) && (in.ymin > out.ymin) && (in.ymax < out.ymax)) rc = true;
    return(rc);
}

uint16_t geometry::getLength(const uint16_t coord_min, const uint16_t coord_max) {
    return(coord_max - coord_min + 1);
}

std::vector<struct data::rect> geometry::splitRect(const struct data::rect orig_rect) {
    std::vector<struct data::rect> rect_vec;
    struct data::rect rect_high, rect_low;
    // Higher rectangle
    rect_high.xmin = orig_rect.xmin;
    rect_high.xmax = orig_rect.xmax;
    rect_high.ymin = orig_rect.ymin;
    rect_high.ymax = orig_rect.ymin + 0.5 * orig_rect.height;
    rect_high.width = getLength(rect_high.xmin, rect_high.xmax);
    rect_high.height = getLength(rect_high.ymin, rect_high.ymax);
    // Lower rectangle
    rect_low.xmin = orig_rect.xmin;
    rect_low.xmax = orig_rect.xmax;
    rect_low.ymin = orig_rect.ymin + 0.5 * orig_rect.height;
    rect_low.ymax = orig_rect.ymax;
    rect_low.width = getLength(rect_low.xmin, rect_low.xmax);
    rect_low.height = getLength(rect_low.ymin, rect_low.ymax);
    // Rectangle vector
    rect_vec.push_back(rect_high);
    rect_vec.push_back(rect_low);
    return(rect_vec);
}

