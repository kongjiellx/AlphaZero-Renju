//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_EXAMINER_H
#define ALPHAZERO_RENJU_EXAMINER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/model_manager.h"
#include <chrono>

class Examiner: public Thread {
    void run();
    void stop();
};


#endif //ALPHAZERO_RENJU_EXAMINER_H
