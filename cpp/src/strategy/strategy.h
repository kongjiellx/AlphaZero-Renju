//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_STRATEGY_H
#define ALPHAZERO_RENJU_STRATEGY_H

#include <tuple>
#include "cpp/src/data_structure/data_structure.h"
#include "cpp/src/board.h"


class Strategy {
protected:
    Player player;
public:
    Strategy(Player player) : player(player) {}

    Player getPlayer() const {
        return player;
    }

    virtual std::tuple<int, int> step(const Board &board, StepRecord &record) = 0;

    virtual void post_process(const Board &board) = 0;

};

#endif //ALPHAZERO_RENJU_STRATEGY_H
