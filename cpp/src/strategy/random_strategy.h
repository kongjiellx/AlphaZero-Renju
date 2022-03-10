#ifndef ALPHAZERO_RENJU_RANDOM_STRATEGY_H
#define ALPHAZERO_RENJU_RANDOM_STRATEGY_H

#include <random>
#include "strategy.h"

class RandomStrategy : public Strategy {
private:
    std::mt19937 rng;
public:
    RandomStrategy(Player player);

    std::tuple<int, int> step(const Board &board, StepRecord &record) override;

    void post_process(const Board &board) override;
};


#endif //ALPHAZERO_RENJU_RANDOM_STRATEGY_H
