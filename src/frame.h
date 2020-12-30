#pragma once

// Local modules
#include "behav.h"

namespace frame {
    bool isStart(const uint32_t frame_num);
    bool isEnd(const uint32_t frame_num);
    bool isCNNActive(const uint32_t frame_num);
    bool isReallyOld(const uint32_t current, const uint32_t last);
    bool isSplitActive(const uint32_t frame_num);
    bool isTrackActive(const uint32_t frame_num);
}
