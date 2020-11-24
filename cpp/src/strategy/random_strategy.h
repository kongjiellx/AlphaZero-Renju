//
// Created by 刘也宽 on 2020/11/18.
//

#ifndef ALPHAZERO_RENJU_RANDOM_STRATEGY_H
#define ALPHAZERO_RENJU_RANDOM_STRATEGY_H

#include <random>
#include "strategy.h"

class RandomStrategy: public Strategy {
private:
    std::mt19937 rng;
public:
    RandomStrategy();

    std::tuple<int, int> step(const Board& board) override;

    void post_process(const Board &board) override;
};


#endif //ALPHAZERO_RENJU_RANDOM_STRATEGY_H
