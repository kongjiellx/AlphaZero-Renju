//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_PIT_H
#define ALPHAZERO_RENJU_PIT_H

#include "cpp/src/strategy/strategy.h"
#include "cpp/src/data_structure/data_structure.h"

class Pit {
public:
    GameResult play_a_game(Strategy *p1, Strategy *p2);
};


#endif //ALPHAZERO_RENJU_PIT_H
