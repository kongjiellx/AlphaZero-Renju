//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PRODUCER_H
#define ALPHAZERO_RENJU_PRODUCER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/resource_manager.h"

class Producer: Thread {
public:
    void init_data_pool() {
        ResourceManager::instance().get_data_pool();
    }

    void run() {
        ResourceManager::instance()
    }
};


#endif //ALPHAZERO_RENJU_PRODUCER_H
