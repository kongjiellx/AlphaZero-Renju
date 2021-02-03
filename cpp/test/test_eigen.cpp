//
// Created by 刘也宽 on 2021/1/24.
//

#include "cpp/src/third_party/eigen-3.3.9/Eigen/Dense"
#include "cpp/src/third_party/eigen-3.3.9/Eigen/Geometry"
#include <iostream>

int main() {
    Eigen::Matrix<float, 3, 2> matrix;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            matrix << 2 * i + j;
            std::cout << matrix(i, j);
        }
        std::cout << std::endl;
    }

    auto t = matrix.transpose().colwise().reverse();
    for (int i = 0; i < t.rows(); i++) {
        for (int j = 0; j < t.cols(); j++) {
            std::cout << t(i,j);
        }
        std::cout << std::endl;
    }
    return 0;
}