//
// Created by 刘也宽 on 2020/10/3.
//
#include "examiner.h"
#include "glog/logging.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/resource_manager.h"
//#include "cpp/src/model_manager.h"
#include <chrono>

void Examiner::run() {
    LOG(INFO) << "examiner start";
    Pit pit;
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(10));
        auto stg1 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::O);
        auto stg2 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::X);
        auto result = pit.play_a_game(stg1, stg2, true);
        if (result->winner == Player::O) {
//            ModelManager::instance().reset_train_model();
        } else if (result->winner == Player::X) {
//            ModelManager::instance().update_predict_model();
        }
    }
}

void Examiner::stop() {}
