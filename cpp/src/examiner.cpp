//
// Created by 刘也宽 on 2020/10/3.
//
#include "examiner.h"

void Examiner::run() {
    Pit pit;
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(5));
        MctsStrategy old_stg(MODEL_TYPE::PREDICT);
        MctsStrategy new_stg(MODEL_TYPE::TRAIN);
        GameResult result = pit.play_a_game(old_stg, new_stg);
        if (result.winner == WINNER::P1_WIN) {
            ModelManager::instance().reset_train_model();
        } else if (result.winner == WINNER::P2_WIN) {
            ModelManager::instance().update_predict_model();
        }
    }
}

void Examiner::stop() {}
