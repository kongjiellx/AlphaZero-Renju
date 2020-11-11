//
// Created by 刘也宽 on 2020/10/3.
//
#include "strategy.h"
#include "cpp/src/resource_manager.h"

RandomStrategy::RandomStrategy() {
    std::random_device dev;
    rng = std::mt19937(dev());
}

std::tuple<int, int> RandomStrategy::step(const Board& board) {
    int board_size = ResourceManager().instance().get_conf().game_conf().board_size();
    auto legal_idx = board.get_legal_idx();
    auto rand_g = std::uniform_int_distribution<int>(0, legal_idx.size() - 1);
    auto rand_idx = legal_idx[rand_g(rng)];
    return std::tuple<int, int>(rand_idx / board_size, rand_idx % board_size);
}

MctsStrategy::MctsStrategy(MODEL_TYPE model_type) {}

std::tuple<int, int> MctsStrategy::step(const Board& board) {
    return std::tuple<int, int>(0, 0);
}