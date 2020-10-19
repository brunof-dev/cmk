#include "kalman.h"

namespace kalman {
    // State vector
    float X[nx][1];
    float X_old[nx][1];
    float X_pred[nx][1];

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
    // Kalman gain matrix
    float K[nx][nz];
    // Measurement uncertainty matrix
    float R[nz][nz];
    // Measurement vector
    float Z[nz][1];
    /*************************************************************************/
}

void kalman::initStateVec(const float xc, const float yc, const float hx, const float hy) {
    X[0][0] = xc;
    X[1][0] = yc;
    X[2][0] = 0.0f;
    X[3][0] = 0.0f;
    X[4][0] = hx;
    X[5][0] = hy;
}

void kalman::initProcErr() {
    matrix::zeros(Q);
}

void kalman::initEstimateErr() {
    matrix::zeros(P);
}

void kalman::initMeasureErr() {
    matrix::zeros(R);
}

void kalman::init(const float xc, const float yc, const float hx, const float hy) {
    // Init state vector
    initStateVec(xc, yc, hx, hy);

    // Init process noise matrix
    initProcErr();

    // Init estimate uncertainty matrix
    initEstimateErr();

    // Init measure uncertainty matrix
    initMeasureErr();

    // Update previous state vector for next iteration (avoid speed step)
    memcpy(X_old, X, sizeof(X_old));
}

void kalman::getSpeed() {
    float vx = X[0][0] - X_old[0][0];
    float vy = X[1][0] - X_old[1][0];
    X[2][0] = vx;
    X[3][0] = vy;
}

void kalman::getProcErr(const float hx, const float hy) {
    Q[0][0] = hx;
    Q[1][1] = hy;
    Q[4][4] = behav::SCALE_ERR_RATE_PROC * hx;
    Q[5][5] = behav::SCALE_ERR_RATE_PROC * hy;
}

void kalman::predictEstimateErr() {
    float F_t[nx][nx], temp1[nx][nx], temp2[nx][nx];
    matrix::transpose(F, F_t);
    matrix::mult(P, F_t, temp1);
    matrix::mult(F, temp1, temp2);
    matrix::plus(temp2, Q, P_pred);
}

void kalman::predictStateVec() {
    matrix::mult(F, X, X_pred);
}

void kalman::predict(const float hx, const float hy) {
    // Update velocity in state vector
    getSpeed();

    // Update process noise matrix
    getProcErr(hx, hy);

    // Predict new state vector
    predictStateVec();

    // Predict new estimate uncertainty matrix
    predictEstimateErr();

    // Update previous state vector for next iteration
    memcpy(X_old, X, sizeof(X_old));

    // Debug
    // matrix::print("X", X);
    // matrix::print("P", P);
    // matrix::print("X_pred", X_pred);
    // matrix::print("P_pred", P_pred);
}

void kalman::getMeasureErr(const float simi, const float hx, const float hy) {
    R[0][0] = (1.0f - simi) * behav::POS_ERR * behav::POS_ERR;
    R[1][1] = (1.0f - simi) * behav::POS_ERR * behav::POS_ERR;
    R[2][2] = (1.0f - simi) * behav::SCALE_ERR_RATE_MEASURE * hx * hx;
    R[3][3] = (1.0f - simi) * behav::SCALE_ERR_RATE_MEASURE * hy * hy;
}

void kalman::getKalmanGain() {
    float H_t[nx][nz], temp1[nx][nz], temp2[nz][nz], temp3[nz][nz], temp4[nz][nz], temp5[nx][nz];
    matrix::transpose(H, H_t);
    matrix::mult(P_pred, H_t, temp1);
    matrix::mult(H, temp1, temp2);
    matrix::plus(temp2, R, temp3);
    matrix::inv(temp3, temp4);
    matrix::mult(H_t, temp4, temp5);
    matrix::mult(P_pred, temp5, K);
}

void kalman::getMeasureVec(const float xc, const float yc, const float hx, const float hy) {
    Z[0][0] = xc;
    Z[1][0] = yc;
    Z[2][0] = hx;
    Z[3][0] = hy;
}

void kalman::updateStateVec() {
    float temp1[nz][1], temp2[nz][1], temp3[nx][1];
    matrix::mult(H, X_pred, temp1);
    matrix::minus(Z, temp1, temp2);
    matrix::mult(K, temp2, temp3);
    matrix::plus(X_pred, temp3, X);
}

void kalman::updateEstimateErr() {
    float temp1[nx][nx], temp2[nx][nx], temp3[nx][nx];
    matrix::mult(K, H, temp1);
    matrix::identity(temp2);
    matrix::minus(temp2, temp1, temp3);
    matrix::mult(temp3, P_pred, P);
}

void kalman::transfer(float& xc, float& yc, float& hx, float& hy) {
    xc = X[0][0];
    yc = X[1][0];
    hx = X[4][0];
    hy = X[5][0];
}

void kalman::update(float& xc, float& yc, float& hx, float& hy, const float simi) {
    // Update measure uncertainty matrix
    getMeasureErr(simi, hx, hy);

    // Update Kalman Gain
    getKalmanGain();

    // Build measurement vector
    getMeasureVec(xc, yc, hx, hy);

    // Update state vector
    updateStateVec();

    // Update estimate uncertainty matrix
    updateEstimateErr();

    // Transfer results from state domain to coordinate domain
    transfer(xc, yc, hx, hy);

    // Debug
    // matrix::print("K", K);
    // matrix::print("Z", Z);
    // matrix::print("X_pred", X_pred);
    // matrix::print("X", X);
    // matrix::print("P", P);
}
