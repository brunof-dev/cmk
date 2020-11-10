#include "kalman.h"

// Constructor
Kalman::Kalman() {
    m_init = false;
}

// Getters
bool Kalman::isInit(void) const { return(m_init); }

// Setters
void Kalman::setInit() { m_init = true; }
void Kalman::clearInit() { m_init = false; }

// General functions
void Kalman::initStateVec(const float xc, const float yc, const float hx, const float hy) {
    X[0][0] = xc;
    X[1][0] = yc;
    X[2][0] = 0.0f;
    X[3][0] = 0.0f;
    X[4][0] = hx;
    X[5][0] = hy;
}

void Kalman::initProcErr() {
    matrix::zeros(Q);
}

void Kalman::initEstimateErr() {
    matrix::zeros(P);
}

void Kalman::initMeasureErr() {
    matrix::zeros(R);
}

void Kalman::init(const float xc, const float yc, const float hx, const float hy) {
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

    // Update init flag
    setInit();
}

void Kalman::getSpeed() {
    float vx = X[0][0] - X_old[0][0];
    float vy = X[1][0] - X_old[1][0];
    X[2][0] = vx;
    X[3][0] = vy;
}

void Kalman::getProcErr(const float hx, const float hy) {
    Q[0][0] = hx;
    Q[1][1] = hy;
    Q[4][4] = behav::SCALE_ERR_RATE_PROC * hx;
    Q[5][5] = behav::SCALE_ERR_RATE_PROC * hy;
}

void Kalman::predictEstimateErr() {
    float F_t[nx][nx], temp1[nx][nx], temp2[nx][nx];
    matrix::transpose(F, F_t);
    matrix::mult(P, F_t, temp1);
    matrix::mult(F, temp1, temp2);
    matrix::plus(temp2, Q, P_pred);
}

void Kalman::predictStateVec() {
    matrix::mult(F, X, X_pred);
}

void Kalman::predict(const float hx, const float hy) {
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
}

void Kalman::getMeasureErr(const float simi, const float hx, const float hy) {
    R[0][0] = (1.0f - simi) * behav::POS_ERR * behav::POS_ERR;
    R[1][1] = (1.0f - simi) * behav::POS_ERR * behav::POS_ERR;
    R[2][2] = (1.0f - simi) * behav::SCALE_ERR_RATE_MEASURE * hx * hx;
    R[3][3] = (1.0f - simi) * behav::SCALE_ERR_RATE_MEASURE * hy * hy;
}

void Kalman::getKalmanGain() {
    float H_t[nx][nz], temp1[nx][nz], temp2[nz][nz], temp3[nz][nz], temp4[nz][nz], temp5[nx][nz];
    matrix::transpose(H, H_t);
    matrix::mult(P_pred, H_t, temp1);
    matrix::mult(H, temp1, temp2);
    matrix::plus(temp2, R, temp3);
    matrix::inv(temp3, temp4);
    matrix::mult(H_t, temp4, temp5);
    matrix::mult(P_pred, temp5, K);
}

void Kalman::getMeasureVec(const float xc, const float yc, const float hx, const float hy) {
    Z[0][0] = xc;
    Z[1][0] = yc;
    Z[2][0] = hx;
    Z[3][0] = hy;
}

void Kalman::updateStateVec() {
    float temp1[nz][1], temp2[nz][1], temp3[nx][1];
    matrix::mult(H, X_pred, temp1);
    matrix::minus(Z, temp1, temp2);
    matrix::mult(K, temp2, temp3);
    matrix::plus(X_pred, temp3, X);
}

void Kalman::updateEstimateErr() {
    float temp1[nx][nx], temp2[nx][nx], temp3[nx][nx];
    matrix::mult(K, H, temp1);
    matrix::identity(temp2);
    matrix::minus(temp2, temp1, temp3);
    matrix::mult(temp3, P_pred, P);
}

void Kalman::transfer(float& xc, float& yc, float& hx, float& hy) {
    xc = X[0][0];
    yc = X[1][0];
    hx = X[4][0];
    hy = X[5][0];
}

void Kalman::update(float& xc, float& yc, float& hx, float& hy, const float simi) {
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
}
