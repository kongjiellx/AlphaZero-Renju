//
// Created by 刘也宽 on 2021/1/24.
//

#include "third_party/eigen3/Eigen/Core"
#include "third_party/eigen3/Eigen/LU"
#include "third_party/eigen3/Eigen/Cholesky"
#include "third_party/eigen3/Eigen/QR"
#include "third_party/eigen3/Eigen/SVD"
#include "third_party/eigen3/Eigen/Eigenvalues"
#include <iostream>

int main() {
    Eigen::MatrixXf matA(2, 2);
    matA << 1, 2, 3, 4;
    int dd = 0;
    
    auto t = matA.transpose().colwise().reverse();
    for (int i = 0; i < t.rows(); i++) {
        for (int j = 0; j < t.cols(); j++) {
            std::cout << t(i,j) << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}