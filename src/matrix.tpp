template <typename T, uint8_t rows, uint8_t cols>
uint8_t matrix::getRows(const T (&mat)[rows][cols]) {
    return(sizeof(mat) / sizeof(mat[0]));
}
template <typename T, uint8_t rows, uint8_t cols>
uint8_t matrix::getCols(const T (&mat)[rows][cols]) {
    return(sizeof(mat[0]) / sizeof(mat[0][0]));
}

template <typename T, uint8_t rows, uint8_t cols>
void matrix::print(const char* const name, const T (&mat)[rows][cols]) {
    std::printf("%-8s = ", name);
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            if ((i == 0) && (j == 0)) std::printf("|%8.2f", mat[i][j]);
            else if (j == 0) std::printf("           |%8.2f", mat[i][j]);
            else std::printf("%8.2f", mat[i][j]);
        }
        std::printf("%4s\n", "|");
    }
    std::printf("\n");
}

template <typename T, uint8_t rows>
void matrix::identity(T (&mat)[rows][rows]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < rows; j++) {
            if (i == j) mat[i][j] = static_cast<T>(1);
            else mat[i][j] = static_cast<T>(0);
        }
    }
}

template <typename T, uint8_t rows>
void matrix::zeros(T (&mat)[rows][rows]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < rows; j++) {
            mat[i][j] = static_cast<T>(0);
        }
    }
}

template <typename T, uint8_t rows, uint8_t cols>
void matrix::transpose(const T (&in_mat)[rows][cols], T (&out_mat)[cols][rows]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            out_mat[j][i] = in_mat[i][j];
        }
    }
}

template <typename T, uint8_t rows, uint8_t cols>
void matrix::plus(const T (&A_mat)[rows][cols], const T (&B_mat)[rows][cols], T (&out_mat)[rows][cols]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            out_mat[i][j] = A_mat[i][j] + B_mat[i][j];
        }
    }
}

template <typename T, uint8_t rows, uint8_t cols>
void matrix::minus(const T (&A_mat)[rows][cols], const T (&B_mat)[rows][cols], T (&out_mat)[rows][cols]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            out_mat[i][j] = A_mat[i][j] - B_mat[i][j];
        }
    }
}

template <typename T, uint8_t rows, uint8_t cols>
void matrix::scale(const T (&in_mat)[rows][cols], const T scalar, T (&out_mat)[rows][cols]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            out_mat[i][j] = in_mat[i][j] * scalar;
        }
    }
}

template <typename T, uint8_t rows_a, uint8_t cols, uint8_t cols_b>
void matrix::mult(const T (&A_mat)[rows_a][cols], const T (&B_mat)[cols][cols_b], T (&out_mat)[rows_a][cols_b]) {
    for (uint8_t i = 0; i < rows_a; i++) {
        for (uint8_t j = 0; j < cols_b; j++) {
            T element = 0;
            for (uint8_t k = 0; k < cols; k++) {
                element += A_mat[i][k] * B_mat[k][j];
            }
            out_mat[i][j] = element;
        }
    }
}

template <typename T, uint8_t rows>
T matrix::det3x3(const T (&mat)[rows][rows]) {
    T det_res = mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0] + mat[0][2] * mat[1][0] * mat[2][1] -\
                mat[2][0] * mat[1][1] * mat[0][2] - mat[2][1] * mat[1][2] * mat[0][0] - mat[2][2] * mat[1][0] * mat[0][1];
    return(det_res);
}

template <typename T, uint8_t rows>
void matrix::getSubMat(const T (&in_mat)[rows][rows], const uint8_t pos_i, const uint8_t pos_j, T (&out_mat)[rows][rows]) {
    uint8_t out_i = 0, out_j = 0;
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < rows; j++) {
            if ((i != pos_i) && (j != pos_j)) {
                out_mat[out_i][out_j] = in_mat[i][j];
                out_j += 1;
                if (out_j == rows -1) {
                    out_j = 0;
                    out_i += 1;
                }
            }
        }
    }
}

template <typename T, uint8_t rows>
T matrix::cof(const T (&mat)[rows][rows], const uint8_t pos_i, const uint8_t pos_j, uint8_t order) {
    T sign = -1.0f;
    if (((pos_i + pos_j) % 2) == 0) sign = 1.0f;
    T sub_mat[rows][rows];
    getSubMat(mat, pos_i, pos_j, sub_mat);
    T cofactor = sign * det(sub_mat, order);
    return(cofactor);
}

template <typename T, uint8_t rows>
T matrix::det(const T (&mat)[rows][rows], uint8_t order) {
    T det_res = 0;
    if (order == 3) det_res = det3x3(mat);
    else {
        for (uint8_t j = 0; j < order; j++) {
            T aij = mat[0][j];
            T cij = cof(mat, 0, j, order - 1);
            det_res += aij * cij;
        }
    }
    return(det_res);
}

template <typename T, uint8_t rows>
void matrix::cofMat(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]) {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < rows; j++) {
            out_mat[i][j] = cof(in_mat, i, j, rows - 1);
        }
    }
}

template <typename T, uint8_t rows>
void matrix::adj(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]) {
    T cof_mat[rows][rows];
    cofMat(in_mat, cof_mat);
    transpose(cof_mat, out_mat);
}

template <typename T, uint8_t rows>
void matrix::inv(const T (&in_mat)[rows][rows], T (&out_mat)[rows][rows]) {
    T det_res = det(in_mat);
    T scalar = static_cast<T>(1) / det_res;
    T adj_mat[rows][rows];
    adj(in_mat, adj_mat);
    scale(adj_mat, scalar, out_mat);
}
