#include "random_strategy.h"

RandomStrategy::RandomStrategy(Player player) : Strategy(player) {
    std::random_device dev;
    rng = std::mt19937(dev());
}

std::tuple<int, int> RandomStrategy::step(const Board &board, StepRecord &record) {
    int board_size = board.get_size();
    auto legal_idx = board.get_legal_idx();
    auto rand_g = std::uniform_int_distribution<int>(0, legal_idx.size() - 1);
    auto rand_idx = legal_idx[rand_g(rng)];
    return std::tuple<int, int>(rand_idx / board_size, rand_idx % board_size);
}

void RandomStrategy::post_process(const Board &board) {
}
