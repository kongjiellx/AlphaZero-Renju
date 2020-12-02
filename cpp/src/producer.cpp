//
// Created by 刘也宽 on 2020/10/3.
//
#include "producer.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "glog/logging.h"
#include "data_structure/data_structure.h"

Producer::Producer(int thread_pool_size) : thread_pool_size(thread_pool_size), total_produce_num(0){
    LOG(INFO) << "start init data pool";
    init_data_pool();
    LOG(INFO) << "init data pool: " << ResourceManager::instance().get_data_pool().get_real_size();
}

void Producer::produce_one() {
    auto& conf = ResourceManager::instance().get_conf();
    MctsStrategy stg1(conf.mtcs_conf(), Player::O);
    MctsStrategy stg2(conf.mtcs_conf(), Player::X);
    GameResult result = pit.play_a_game((Strategy*)&stg1, (Strategy*)&stg2, false);
    total_produce_num += result.records.size();
    for(auto instance: game_result_to_instances(result)) {
        ResourceManager::instance().get_data_pool().push_back(instance);
    }
    LOG_EVERY_N(INFO, 100) << "total_produce_num: " << total_produce_num;
}

void Producer::produce_endless() {
    while(true) {
        produce_one();
    }
}

void Producer::init_data_pool() {
    ThreadPool thread_pool(thread_pool_size);
    std::vector<std::future<void>> futures;
    while(true) {
        for(size_t i = 0; i < thread_pool_size; i++) {
            futures.emplace_back(thread_pool.enqueue(&Producer::produce_one, this));
        }
        for (auto&& future: futures) {
            future.get();
        }
        futures.clear();
        if (ResourceManager::instance().get_data_pool().full()) {
            LOG(INFO) << "init data pool done!";
            return;
        }
    }
}

void Producer::run() {
    LOG(INFO) << "producer start!";
    ThreadPool thread_pool(thread_pool_size);
    std::vector<std::future<void>> futures;
    for(size_t i = 0; i < thread_pool_size; i++) {
        futures.emplace_back(thread_pool.enqueue(&Producer::produce_endless, this));
    }
}

Producer::~Producer() {

}

void Producer::stop() {

}
