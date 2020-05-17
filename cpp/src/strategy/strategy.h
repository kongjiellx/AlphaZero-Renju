//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_STRATEGY_H
#define ALPHAZERO_RENJU_STRATEGY_H

#include <tuple>

class Strategy {
public:
    virtual std::tuple<int, int> step() = 0;
};

#endif //ALPHAZERO_RENJU_STRATEGY_H
