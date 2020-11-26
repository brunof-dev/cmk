#pragma once

// General modules
#include <cstdint>
#include <string>

namespace behav {
    // General behavior
    /*************************************************************************/
    const std::string INPUT_VID = "../data/MOT17-09.webm";
    const uint32_t FRAME_START = 1;
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

    // Neural network
    /*************************************************************************/
    const std::string DEVICE = "CPU";
    const std::string INPUT_BLOB = "image_tensor";
    const std::string MODEL_XML = "/home/bruno/openvino_models/ir/public/ssdlite_mobilenet_v2/FP32/ssdlite_mobilenet_v2.xml";
    const std::string MODEL_BIN = "/home/bruno/openvino_models/ir/public/ssdlite_mobilenet_v2/FP32/ssdlite_mobilenet_v2.bin";
    const std::string OUTPUT_BLOB = "DetectionOutput";
    const int16_t END_CODE = -1;
    const uint16_t HUMAN = 1;
    const float CONF_LEVEL = 0.0f;
    const uint16_t MAX_WIDTH = 300;
    const uint16_t MAX_HEIGHT = 600;
    const float IOU = 0.25;
    /*************************************************************************/

    // CMK
    /*************************************************************************/
    const uint16_t STACK_SIZE = 5;
    const uint16_t DIST = 300;
    const int32_t INVALID_ID = -1;
    const int32_t INVALID_NB_INDEX = -1;
    /*************************************************************************/

    // Drawing
    /*************************************************************************/
    const uint16_t TXT_OFFSET = 10;
    /*************************************************************************/
}
