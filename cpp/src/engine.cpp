//
// Created by liuyekuan on 2020/5/5.
//

#include "engine.h"

Engine::Engine() {
    ResourceManager::instance();
    ModelManager::instance().init();
    if (ResourceManager::instance().get_conf().model_conf().load_pre()) {
        LOG(INFO) << "load pre trained model!";
        ModelManager::instance().load_model(MODEL_TYPE::TRAIN);
        ModelManager::instance().load_model(MODEL_TYPE::PREDICT);
    }
}

void Engine::add_workers() {
    workers.push_back(make_shared<Producer>(13));
    workers.push_back(make_shared<Trainer>());
    workers.push_back(make_shared<Examiner>(13, 50));
}

void Engine::start() {
    for (auto &thread: workers) {
        thread->start();
    }
    for (auto &thread: workers) {
        thread->join();
    }
}

void Engine::stop() {
    for (auto &thread: workers) {
        thread->stop();
    }
}
