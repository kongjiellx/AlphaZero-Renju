//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_TRAINER_H
#define ALPHAZERO_RENJU_TRAINER_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/data_structure/data_structure.h"

class Trainer: public Thread {
public:
    void run() {
        int batch_size = ResourceManager::instance().get_conf().model_conf().batch_size();
        while (true) {
            std::vector<Instance> instances = ResourceManager::instance().get_data_pool().sample_batch(batch_size);
            ModelManager::instance().train_on_batch(instances);
        }
    }

    void stop() {}
};


#endif //ALPHAZERO_RENJU_TRAINER_H
