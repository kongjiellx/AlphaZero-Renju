//
// Created by 刘也宽 on 2020/10/3.
//
#include "strategy.h"
#include "cpp/src/resource_manager.h"

RandomStrategy::RandomStrategy() {
    std::random_device dev;
    rng = std::mt19937(dev());
}

std::tuple<int, int> RandomStrategy::step() {
    int board_size = ResourceManager().instance().get_conf().game_conf().board_size();
    auto rand_g = std::uniform_int_distribution<std::mt19937::result_type>(0, board_size - 1);
    return std::tuple<int, int>(rand_g(rng), rand_g(rng));
}

MctsStrategy::MctsStrategy(MODEL_TYPE model_type) {}

std::tuple<int, int> MctsStrategy::step() {
    return std::tuple<int, int>(0, 0);
}