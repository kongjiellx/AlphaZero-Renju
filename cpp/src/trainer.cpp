//
// Created by 刘也宽 on 2020/10/3.
//
#include "trainer.h"

void Trainer::run() {
    int batch_size = ResourceManager::instance().get_conf().model_conf().batch_size();
    while (true) {
        std::vector<Instance> instances = ResourceManager::instance().get_data_pool().sample_batch(batch_size);
        ModelManager::instance().train_on_batch(instances);
    }
}

void Trainer::stop() {}
