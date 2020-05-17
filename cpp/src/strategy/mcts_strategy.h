//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_MCTS_STRATEGY_H
#define ALPHAZERO_RENJU_MCTS_STRATEGY_H

#include "cpp/src/strategy/strategy.h"
#include "cpp/src/data_structure/data_structure.h"

class MctsStrategy: public Strategy {
public:
    MctsStrategy(MODEL_TYPE model_type) {}

    std::tuple<int, int> step() override {
        return std::tuple<int, int>(0, 0);
    }
};


#endif //ALPHAZERO_RENJU_MCTS_STRATEGY_H
