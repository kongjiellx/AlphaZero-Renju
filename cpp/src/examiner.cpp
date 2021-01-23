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

void Examiner::run() {
    LOG(INFO) << "examiner start";
    Pit pit;
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(10));
        auto stg1 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::O, MODEL_TYPE::TRAIN);
        auto stg2 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::X, MODEL_TYPE::PREDICT);

        int o_win = 0, x_win = 0;
        for (int i = 0; i < 10; i++) {
            LOG(INFO) << "game[" << i + 1 << "]";
            auto result = pit.play_a_game(stg1, stg2, true);
            if (result->winner == Player::O) {
                o_win++;
            } else if (result->winner == Player::X) {
                x_win++;
            }
        }
        if (o_win > x_win) {
            LOG(INFO) << "winner: O, update predict model!";
            ModelManager::instance().update_predict_model();
        } else {
            LOG(INFO) << "winner: X, reset train model!";
            ModelManager::instance().reset_train_model();
        }
    }
}

void Examiner::stop() {}
