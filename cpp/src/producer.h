//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PRODUCER_H
#define ALPHAZERO_RENJU_PRODUCER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread_pool.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/pit.h"

class Producer: public Thread {
private:
    int thread_pool_size;
    Pit pit;
public:
    explicit Producer(int thread_pool_size) : thread_pool_size(thread_pool_size){
        init_data_pool();
    }

    void play_a_game() {
        MctsStrategy stg1(MODEL_TYPE::PREDICT);
        MctsStrategy stg2(MODEL_TYPE::PREDICT);
        GameResult result = pit.play_a_game(stg1, stg2);
        for(auto instance: game_result_to_instances(result)) {
            ResourceManager::instance().get_data_pool().push_back(instance);
        }
    }

    void play_endless() {
        MctsStrategy stg1(MODEL_TYPE::PREDICT);
        MctsStrategy stg2(MODEL_TYPE::PREDICT);
        while(true) {
            GameResult result = pit.play_a_game(stg1, stg2);
            for (auto instance: game_result_to_instances(result)) {
                ResourceManager::instance().get_data_pool().push_back(instance);
            }
        }
    }

    void init_data_pool() {
        FixedDeque<Instance>& pool = ResourceManager::instance().get_data_pool();
        ThreadPool thread_pool(thread_pool_size);
        std::vector<std::future<void>> futures;
        while(true) {
            for(size_t i = 0; i < thread_pool_size; i++) {
                futures.emplace_back(thread_pool.enqueue(&Producer::play_a_game, this));
            }
            for (auto&& future: futures) {
                future.get();
            }
            if (ResourceManager::instance().get_data_pool().full()) {
                std::cout << "init data pool done!" << std::endl;
                return;
            }
        }
    }

    void run() {
        FixedDeque<Instance>& pool = ResourceManager::instance().get_data_pool();
        ThreadPool thread_pool(thread_pool_size);
        std::vector<std::future<void>> futures;
        for(size_t i = 0; i < thread_pool_size; i++) {
            futures.emplace_back(thread_pool.enqueue(&Producer::play_endless, this));
        }
    }

    void stop() {}
};


#endif //ALPHAZERO_RENJU_PRODUCER_H
