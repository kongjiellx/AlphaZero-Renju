#include "producer.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "data_structure/data_structure.h"
#include "cpp/src/strategy/model_expert.h"
#include "spdlog/spdlog.h"

Producer::Producer(int thread_pool_size) : thread_pool_size(thread_pool_size), total_produce_num(0), total_game_num(0) {
    spdlog::info("start init data pool");
    init_data_pool();
    spdlog::info("init data pool: " + std::to_string(ResourceManager::instance().get_data_pool().get_real_size()));
}

void Producer::produce_one() {
    auto &conf = ResourceManager::instance().get_conf();
    auto model_expert = make_shared<ModelExpert>(true, MODEL_TYPE::PREDICT);
    auto stg1 = make_shared<MctsStrategy>(conf.mtcs_conf(), Player::O, model_expert, true);
    auto stg2 = make_shared<MctsStrategy>(conf.mtcs_conf(), Player::X, model_expert, true);
    auto result = pit.play_a_game(stg1, stg2, false);
    total_produce_num += result->records.size() * 8;
    auto instances = game_result_to_instances(result);
    for (auto &instance: *instances) {
        ResourceManager::instance().get_data_pool().push_back(instance);
    }
    total_game_num += 1;
    if (total_game_num % conf.self_play_conf().producer_log_freq() == 0) {
        spdlog::info("total_game_num: " + std::to_string(total_game_num) + ", total_produce_num:" + std::to_string(total_produce_num));
    }
    
}

void Producer::produce_endless() {
    while (true) {
        produce_one();
    }
}

void Producer::init_data_pool() {
    ThreadPool thread_pool(thread_pool_size);
    std::vector<std::future<void>> futures;
    while (true) {
        for (size_t i = 0; i < thread_pool_size; i++) {
            futures.emplace_back(thread_pool.enqueue(&Producer::produce_one, this));
        }
        for (auto &&future: futures) {
            future.get();
        }
        futures.clear();
        if (ResourceManager::instance().get_data_pool().full()) {
            spdlog::info("init data pool done!");
            return;
        }
    }
}

void Producer::run() {
    spdlog::info("producer start!");
    ThreadPool thread_pool(thread_pool_size);
    std::vector<std::future<void>> futures;
    for (size_t i = 0; i < thread_pool_size; i++) {
        futures.emplace_back(thread_pool.enqueue(&Producer::produce_endless, this));
    }
}

Producer::~Producer() {

}

void Producer::stop() {

}
