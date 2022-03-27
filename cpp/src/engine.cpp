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
    auto self_play_conf = ResourceManager::instance().get_conf().self_play_conf();
    workers.push_back(make_shared<Producer>(self_play_conf.producer_threads_num()));
    workers.push_back(make_shared<Trainer>());
    workers.push_back(make_shared<Examiner>(self_play_conf.examiner_threads_num(), self_play_conf.examiner_game_num()));
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
