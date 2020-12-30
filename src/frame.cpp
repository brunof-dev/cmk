#include "frame.h"

bool frame::isStart(const uint32_t frame_num) {
    bool rc = false;
    if (frame_num >= behav::FRAME_START) rc = true;
    return(rc);
}

bool frame::isEnd(const uint32_t frame_num) {
    bool rc = false;
    if (frame_num >= behav::FRAME_STOP) rc = true;
    return(rc);
}

bool frame::isCNNActive(const uint32_t frame_num) {
    bool rc = false;
    if (((frame_num - behav::FRAME_START) % behav::CNN_FREQ) == 0) rc = true;
    return(rc);
}

bool frame::isReallyOld(const uint32_t current, const uint32_t last) {
    bool rc = false;
    if ((current - last) > behav::STACK_SIZE) rc = true;
    return(rc);
}

bool frame::isSplitActive(const uint32_t frame_num) {
    bool rc = false;
    if (((frame_num - behav::FRAME_START) % behav::SPLIT_FREQ) == 0) rc = true;
    return(rc);
}

bool frame::isTrackActive(const uint32_t frame_num) {
    bool rc = false;
    if (((frame_num - behav::FRAME_START) % behav::TRACK_FREQ) == 0) rc = true;
    return(rc);
}
