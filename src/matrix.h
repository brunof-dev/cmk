#pragma once

// General modules
#include <cstdio>
#include <cstdint>

namespace matrix {
    template <typename T, uint8_t rows, uint8_t cols> uint8_t getCols(const T (&mat)[rows][cols]);
    template <typename T, uint8_t rows, uint8_t cols> uint8_t getRows(const T (&mat)[rows][cols]);
    template <typename T, uint8_t rows, uint8_t cols> void print(const char* const name, const T (&mat)[rows][cols]);
    template <typename T, uint8_t rows> void identity(T (&mat)[rows][rows]);
    template <typename T, uint8_t rows> void zeros(T (&mat)[rows][rows]);
    template <typename T, uint8_t rows, uint8_t cols> void transpose(const T (&in_mat)[rows][cols], T (&out_mat)[cols][rows]);
    template <typename T, uint8_t rows, uint8_t cols> void plus(const T (&A_mat)[rows][cols], const T (&B_mat)[rows][cols],
                                                                T (&out_mat)[rows][cols]);
    template <typename T, uint8_t rows, uint8_t cols> void minus(const T (&A_mat)[rows][cols], const T (&B_mat)[rows][cols],
                                                                 T (&out_mat)[rows][cols]);
    template <typename T, uint8_t rows, uint8_t cols> void scale(const T (&in_mat)[rows][cols], const T scalar,
                                                                 T (&out_mat)[rows][cols]);
    template <typename T, uint8_t rows_a, uint8_t cols, uint8_t cols_b> void mult(const T (&A_mat)[rows_a][cols],
                                                                                  const T (&B_mat)[cols][cols_b],
                                                                                  T (&out_mat)[rows_a][cols_b]);
    template <typename T, uint8_t rows> T det3x3(const T (&mat)[rows][rows]);
    template <typename T, uint8_t rows> void getSubMat(const T (&in_mat)[rows][rows], const uint8_t pos_i, const uint8_t pos_j,
                                                       T (&out_mat)[rows][rows]);
    template <typename T, uint8_t rows> T cof(const T (&mat)[rows][rows], const uint8_t pos_i, const uint8_t pos_j,
                                              uint8_t order);
    template <typename T, uint8_t rows> T det(const T (&mat)[rows][rows], uint8_t order=rows);
    template <typename T, uint8_t rows> void cofMat(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]);
    template <typename T, uint8_t rows> void adj(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]);
    template <typename T, uint8_t rows> void inv(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]);
};

// Template
#include "matrix.tpp"
