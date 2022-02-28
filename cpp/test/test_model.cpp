//
// Created by liuyekuan on 19-11-9.
//

#include "cpp/src/model.h"
#include <vector>

int main() {
    Model model(10);
    model.init("/Users/admin/fish/code/AlphaZero-Renju/cpp/model");
    std::vector<float> x_data;
    std::vector<float> p_data;
    std::vector<float> v_data;

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10 * 10; j++) {
            for (int k = 0; k < 3; k++) {
                x_data.push_back(0.1);
            }
            p_data.push_back(0.01);
        }
        v_data.push_back(1);
    }

    auto ret = model.predict(x_data);
    for (auto i: std::get<0>(*ret)) {
        std::cout << i << ",";
    }
    std::cout << std::endl;
    auto loss = model.train(x_data, p_data, v_data);

    ret = model.predict(x_data);
    for (auto i: std::get<0>(*ret)) {
        std::cout << i << ",";
    }
    std::cout << std::endl;

    model.save("/Users/admin/fish/code/AlphaZero-Renju/cpp/weights");

    model.load("/Users/admin/fish/code/AlphaZero-Renju/cpp/weights");
    ret = model.predict(x_data);
    for (auto i: std::get<0>(*ret)) {
        std::cout << i << ",";
    }
    std::cout << std::endl;

    loss = model.train(x_data, p_data, v_data);

    ret = model.predict(x_data);
    for (auto i: std::get<0>(*ret)) {
        std::cout << i << ",";
    }
    std::cout << std::endl;
    return 0;
}