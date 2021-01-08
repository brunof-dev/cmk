#pragma once

// General modules
#include <cstdint>
#include <string>

namespace behav {
    // General behavior
    /*************************************************************************/
    const std::string INPUT_VID = "../data/MOT17-09.webm";
    extern uint32_t FRAME_START;
    extern bool FRAME_END;
    extern uint32_t FRAME_STOP;
    extern bool MANUAL_STEP;
    const uint8_t WAIT_TIME = 1;
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
    extern bool KALMAN_ON;
    /*************************************************************************/

    // Neural network
    /*************************************************************************/
    const std::string DEVICE = "CPU";
    const std::string INPUT_BLOB = "image_tensor";
    const std::string MODEL_XML = "../data/model.xml";
    const std::string MODEL_BIN = "../data/model.bin";
    const std::string OUTPUT_BLOB = "DetectionOutput";
    const int16_t END_CODE = -1;
    const uint16_t HUMAN = 1;
    const float CONF_LEVEL = 0.0f;
    const uint16_t MAX_WIDTH = 300;
    const uint16_t MAX_HEIGHT = 600;
    const float IOU_SUP = 0.25;
    const uint8_t CNN_FREQ = 1;
    /*************************************************************************/

    // CMK
    /*************************************************************************/
    extern uint16_t STACK_SIZE;
    const uint16_t MAX_NB_DIST = 2500;
    const uint16_t MIN_CMK_DIST = 10;
    const int32_t INVALID_ID = -1;
    const int32_t INVALID_INDEX = -1;
    const uint8_t OCURRENCE = 5;
    const float IOU_SPLIT = 0.15;
    const uint8_t SPLIT_FREQ = 5;
    const uint8_t TRACK_FREQ = 1;
    const uint8_t LOCK_CNT = 4;
    const uint8_t UNLOCK_CNT = 25;
    extern bool CMK_ON;
    extern bool RECOVER_ON;
    /*************************************************************************/

    // Drawing
    /*************************************************************************/
    const uint16_t TXT_OFFSET = 10;
    /*************************************************************************/
}
