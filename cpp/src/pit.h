//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_PIT_H
#define ALPHAZERO_RENJU_PIT_H

#include "cpp/src/strategy/strategy.h"
#include "cpp/src/data_structure/data_structure.h"

class Pit {
public:
    shared_ptr<GameResult> play_a_game(shared_ptr<Strategy> p1, shared_ptr<Strategy> p2, bool print);
};


#endif //ALPHAZERO_RENJU_PIT_H
