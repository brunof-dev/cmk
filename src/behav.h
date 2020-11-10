#pragma once

// General modules
#include <cstdint>
#include <string>

namespace behav {
    // General behavior
    /*************************************************************************/
    const std::string INPUT_VID = "../data/MOT17-09.webm";
    const uint32_t FRAME_START = 80;
    const uint32_t FRAME_STOP = 10000;
    const uint8_t FRAME_GAP = 5;
    const bool KALMAN_ON = true;
    /*************************************************************************/

    // Mean shift
    /*************************************************************************/
    const uint16_t BINS = 4096;
    const float SHIFT_STEP_MIN = 0.1f;
    const uint8_t ITERATION_MAX = 5;
    const float SCALE_STEP = 0.1;
    const float SCALE_ADAPT = 0.7;
    /*************************************************************************/

    // Kalman filter
    /*************************************************************************/
    const float POS_ERR = 5;
    const float SCALE_ERR_RATE_MEASURE = 0.005;
    const float SCALE_ERR_RATE_PROC = 0.7;
    /*************************************************************************/
}
