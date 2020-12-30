#pragma once

// General modules
#include <algorithm>
#include <cassert>

// Local modules
#include "data.h"

namespace geometry {
    uint32_t distance(const struct data::rect a, const struct data::rect b);
    float IOU(const struct data::rect a, const struct data::rect b);
    bool isInner(const struct data::rect in, const struct data::rect out);
    uint16_t getLength(const uint16_t coord_min, const uint16_t coord_max);
}
