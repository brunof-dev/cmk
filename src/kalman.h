#pragma once

// General modules
#include <cstring>

// Local modules
#include "behav.h"
#include "matrix.h"

class Kalman {

    // Number of states
    static const uint8_t nx = 6;
    // Number of measurements
    static const uint8_t nz = 4;
    // State transition matrix
    static constexpr float F[nx][nx] = {
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    };
    // Measurement matrix
    static constexpr float H[nz][nx] = {
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    };

    private:
        bool m_init;

        // Prediction stage
        /*************************************************************************/
        // Estimate uncertainty matrix
        float P[nx][nx];
        float P_pred[nx][nx];
        // Process noise matrix
        float Q[nx][nx];
        /*************************************************************************/

        // Update stage
        /*************************************************************************/
        // Kalmain gain matrix
        float K[nx][nz];
        // Measurement uncertainty matrix
        float R[nz][nz];
        // Measurement vector
        float Z[nz][1];
        /*************************************************************************/

        // State vector
        /*************************************************************************/
        float X[nx][1];
        float X_old[nx][1];
        float X_pred[nx][1];
        /*************************************************************************/

    public:
        // Constructor
        Kalman();

        // Getters
        bool isInit(void) const;

        // Setters
        void setInit(void);
        void clearInit(void);

        // Init stage
        void initStateVec(const float xc, const float yc, const float hx, const float hy);
        void initProcErr(void);
        void initEstimateErr(void);
        void initMeasureErr(void);
        void init(const float xc, const float yc, const float hx, const float hy);

        // Prediction stage
        void getSpeed(void);
        void getProcErr(const float hx, const float hy);
        void predictStateVec(void);
        void predictEstimateErr(void);
        void predict(const float hx, const float hy);

        // Update stage
        void getMeasureErr(const float simi, const float hx, const float hy);
        void getKalmanGain(void);
        void getMeasureVec(const float xc, const float yc, const float hx, const float hy);
        void updateStateVec(void);
        void updateEstimateErr(void);
        void transfer(float& xc, float& yc, float& hx, float& hy);
        void update(float& xc, float& yc, float& hx, float& hy, const float simi);
};
