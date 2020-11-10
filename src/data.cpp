// Local modules
#include "data.h"

// Operators
bool data::operator==(const struct rect rect_a, const struct rect rect_b) {
    bool rc = false;
    if ((rect_a.xmin == rect_b.xmin) && (rect_a.xmax == rect_b.xmax) &&\
        (rect_a.ymin == rect_b.ymin) && (rect_a.ymax == rect_b.ymax)) rc = true;
    return(rc);
}

bool data::operator!=(const struct rect rect_a, const struct rect rect_b) {
    bool rc = false;
    if ((rect_a.xmin != rect_b.xmin) || (rect_a.xmax != rect_b.xmax) ||\
        (rect_a.ymin != rect_b.ymin) || (rect_a.ymax != rect_b.ymax)) rc = true;
    return(rc);
}

bool data::operator==(const struct ellipse ellipse_a, const struct ellipse ellipse_b) {
    bool rc = false;
    if ((ellipse_a.xc == ellipse_b.xc) && (ellipse_a.yc == ellipse_b.yc) &&\
        (ellipse_a.hx == ellipse_b.hx) && (ellipse_a.hy == ellipse_b.hy)) rc = true;
    return(rc);
}

bool data::operator!=(const struct ellipse ellipse_a, const struct ellipse ellipse_b) {
    bool rc = false;
    if ((ellipse_a.xc != ellipse_b.xc) || (ellipse_a.yc != ellipse_b.yc) ||\
        (ellipse_a.hx != ellipse_b.hx) || (ellipse_a.hy != ellipse_b.hy)) rc = true;
    return(rc);
}
