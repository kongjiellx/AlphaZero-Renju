//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PRODUCER_H
#define ALPHAZERO_RENJU_PRODUCER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread_pool.h"

class Producer: Thread {
private:
    int thread_pool_size;
public:
    explicit Producer(int thread_pool_size) : thread_pool_size(thread_pool_size){}

    void play_a_game()
    void init_data_pool() {
        FixedDeque<Instance>& pool = ResourceManager::instance().get_data_pool();
        ThreadPool thread_pool(thread_pool_size);
        while(true) {
            for(size_t i = 0; i < thread_pool_size; i++) {

            }
        }
    }

    void run() {
        ResourceManager::instance()
    }

    void stop() {}
};


#endif //ALPHAZERO_RENJU_PRODUCER_H
