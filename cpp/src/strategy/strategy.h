//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_STRATEGY_H
#define ALPHAZERO_RENJU_STRATEGY_H

#include <tuple>
#include "cpp/src/data_structure/data_structure.h"
#include "cpp/src/board.h"


class Strategy {
public:
    virtual std::tuple<int, int> step(const Board& board) = 0;
};

#endif //ALPHAZERO_RENJU_STRATEGY_H
