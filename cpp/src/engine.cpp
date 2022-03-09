#include "engine.h"
#include "spdlog/spdlog.h"

Engine::Engine() {
    ResourceManager::instance();
    ModelManager::instance().init();
    if (ResourceManager::instance().get_conf().model_conf().load_pre()) {
        spdlog::info("load pre trained model!");
        ModelManager::instance().load_model(MODEL_TYPE::TRAIN);
        ModelManager::instance().load_model(MODEL_TYPE::PREDICT);
    }
}

void Engine::add_workers() {
    workers.push_back(make_shared<Producer>(8));
    workers.push_back(make_shared<Trainer>());
    workers.push_back(make_shared<Examiner>(8, 50));
}

void Engine::start() {
    spdlog::info("Engine start.");
    for (auto &thread: workers) {
        thread->start();
    }
    spdlog::info("Engine started.");
    for (auto &thread: workers) {
        thread->join();
    }
}

void Engine::stop() {
    for (auto &thread: workers) {
        thread->stop();
    }
}
