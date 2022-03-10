#include "matrix.h"

Eigen::MatrixXf rotation90(int n, const Eigen::MatrixXf &matrix) {
    n = n % 4;
    if (n == 0) {
        return matrix;
    } else if (n == 1) {
        return matrix.transpose().rowwise().reverse();
    } else if (n == 2) {
        return matrix.rowwise().reverse().colwise().reverse();
    } else {
        return matrix.transpose().colwise().reverse();
    }
}

Eigen::MatrixXi rotation90(int n, const Eigen::MatrixXi &matrix) {
    n = n % 4;
    if (n == 0) {
        return matrix;
    } else if (n == 1) {
        return matrix.transpose().rowwise().reverse();
    } else if (n == 2) {
        return matrix.rowwise().reverse().colwise().reverse();
    } else {
        return matrix.transpose().colwise().reverse();
    }
}
