//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PRODUCER_H
#define ALPHAZERO_RENJU_PRODUCER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/utils/thread_pool.h"
#include "cpp/src/strategy/strategy.h"
#include "cpp/src/pit.h"
#include  <atomic>

class Producer : public Thread {
private:
    int thread_pool_size;
    Pit pit;
    std::atomic<int> total_produce_num;
    std::atomic<int> total_game_num;
public:
    explicit Producer(int thread_pool_size);

    void produce_one();

    void produce_endless();

    void init_data_pool();

    void run() override;

    ~Producer() override;

    void stop() override;
};


#endif //ALPHAZERO_RENJU_PRODUCER_H
