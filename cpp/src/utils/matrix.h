#ifndef ALPHAZERO_RENJU_MATRIX_H
#define ALPHAZERO_RENJU_MATRIX_H

#include "third_party/eigen3/Eigen/Core"
#include "third_party/eigen3/Eigen/LU"
#include "third_party/eigen3/Eigen/Cholesky"
#include "third_party/eigen3/Eigen/QR"
#include "third_party/eigen3/Eigen/SVD"
#include "third_party/eigen3/Eigen/Eigenvalues"


Eigen::MatrixXf rotation90(int n, const Eigen::MatrixXf &matrix);

Eigen::MatrixXi rotation90(int n, const Eigen::MatrixXi &matrix);

#endif //ALPHAZERO_RENJU_MATRIX_H
