//
// Created by liuyekuan on 2020/5/5.
//

#include "engine.h"

Engine::Engine() {
    ResourceManager::instance();
    ModelManager::instance().init();
}

void Engine::add_workers() {
    workers.push_back(new Producer(12));
    workers.push_back(new Trainer());
    workers.push_back(new Examiner());
}

void Engine::start() {
    for (auto& thread: workers) {
        thread -> start();
    }
    for (auto& thread: workers) {
        thread -> join();
    }
}

void Engine::stop() {
    for (auto &thread: workers) {
        thread -> stop();
        delete thread;
    }
}
