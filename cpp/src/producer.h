//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PRODUCER_H
#define ALPHAZERO_RENJU_PRODUCER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread_pool.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/pit.h"

class Producer: public Thread {
private:
    int thread_pool_size;
    Pit pit;
public:
    explicit Producer(int thread_pool_size);

    void play_a_game();

    void play_endless();

    void init_data_pool();

    void run();

    void stop() {}
};


#endif //ALPHAZERO_RENJU_PRODUCER_H
