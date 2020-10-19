#pragma once

// General modules
#include <cstring>

// Local modules
#include "behav.h"
#include "benchmark.h"
#include "matrix.h"

namespace kalman {
    // Number of states
    const uint8_t nx = 6;
    // Number of measurements
    const uint8_t nz = 4;
    // State vector
    extern float X[nx][1];
    extern float X_old[nx][1];
    extern float X_pred[nx][1];

    // Prediction stage
    /*************************************************************************/
    // State transition matrix
    const float F[nx][nx] = {
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    };
    // Estimate uncertainty matrix
    extern float P[nx][nx];
    extern float P_pred[nx][nx];
    // Process noise matrix
    extern float Q[nx][nx];
    /*************************************************************************/

    // Update stage
    /*************************************************************************/
    // Measurement matrix
    const float H[nz][nx] = {
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    };
    // Kalmain gain matrix
    extern float K[nx][nz];
    // Measurement uncertainty matrix
    extern float R[nz][nz];
    // Measurement vector
    extern float Z[nz][1];
    /*************************************************************************/

    // Functions
    /*************************************************************************/
    void initStateVec(const float xc, const float yc, const float hx, const float hy);
    void initProcErr(void);
    void initEstimateErr(void);
    void initMeasureErr(void);
    void init(const float xc, const float yc, const float hx, const float hy);
    void getSpeed(void);
    void getProcErr(const float hx, const float hy);
    void predictStateVec(void);
    void predictEstimateErr(void);
    void predict(const float hx, const float hy);
    void getMeasureErr(const float simi, const float hx, const float hy);
    void getKalmanGain(void);
    void getMeasureVec(const float xc, const float yc, const float hx, const float hy);
    void updateStateVec(void);
    void updateEstimateErr(void);
    void transfer(float& xc, float& yc, float& hx, float& hy);
    void update(float& xc, float& yc, float& hx, float& hy, const float simi);
    /*************************************************************************/
};
