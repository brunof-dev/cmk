#pragma once

// General modules
#include <cstdint>

namespace data {
    struct rect {
        uint16_t xmin;
        uint16_t ymin;
        uint16_t xmax;
        uint16_t ymax;
        float conf;
    };

    struct ellipse {
        float xc;
        float yc;
        float hx;
        float hy;
        float simi;
    };

    // Operators
    bool operator==(const struct rect rect_a, const struct rect rect_b);
    bool operator!=(const struct rect rect_a, const struct rect rect_b);
    bool operator==(const struct ellipse ellipse_a, const struct ellipse ellipse_b);
    bool operator!=(const struct ellipse ellipse_a, const struct ellipse ellipse_b);
};
