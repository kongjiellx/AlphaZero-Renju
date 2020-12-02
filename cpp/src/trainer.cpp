//
// Created by 刘也宽 on 2020/10/3.
//
#include "trainer.h"
#include "glog/logging.h"

void Trainer::run() {
    LOG(INFO) << "trainer started!";
    int batch_size = ResourceManager::instance().get_conf().model_conf().batch_size();

    while (true) {
        std::vector<Instance> instances = ResourceManager::instance().get_data_pool().sample_batch(batch_size);
        ModelManager::instance().train_on_batch(instances);
        total_consume_num += instances.size();
        LOG_EVERY_N(INFO, 1000) << "total_consume_num: " << total_consume_num;
    }
}

void Trainer::stop() {}
