//
// Created by 刘也宽 on 2020/10/3.
//
#include "examiner.h"
#include "glog/logging.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include <chrono>

Examiner::Examiner(int thread_num, int game_num): thread_pool(thread_num), game_num(game_num) {}

void Examiner::run() {
    LOG(INFO) << "examiner start";
    Pit pit;
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(1));

        ModelManager::instance().get_train_model_mutex().Lock();
        ModelManager::instance().get_predict_model_mutex().Lock();
        std::vector<std::future<shared_ptr<GameResult>>> futures;
        for (size_t i = 0; i < game_num; i++) {
            auto stg1 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::O, MODEL_TYPE::TRAIN,
                                                  false);
            auto stg2 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::X, MODEL_TYPE::PREDICT,
                                                  false);
            futures.emplace_back(thread_pool.enqueue(&Pit::play_a_game, &pit, stg1, stg2, i == game_num - 1));
        }
        int o_win = 0, x_win = 0;
        for (auto &&future: futures) {
            auto result = future.get();
            if (result->winner == Player::O) {
                o_win++;
            } else if (result->winner == Player::X) {
                x_win++;
            }
        }

        LOG(INFO) << "O win: " << o_win << ", X win: " << x_win;
        if (o_win > x_win) {
            LOG(INFO) << "winner: O, update predict model!";
            ModelManager::instance().update_predict_model();
        } else {
            LOG(INFO) << "winner: X, reset train model!";
            ModelManager::instance().reset_train_model();
        }
        ModelManager::instance().get_train_model_mutex().Unlock();
        ModelManager::instance().get_predict_model_mutex().Unlock();
    }
}

void Examiner::stop() {}
