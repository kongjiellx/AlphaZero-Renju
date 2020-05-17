//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_EXAMINER_H
#define ALPHAZERO_RENJU_EXAMINER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/model_manager.h"
#include <chrono>

class Examiner: public Thread {
    void run() {
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
    void stop() {}
};


#endif //ALPHAZERO_RENJU_EXAMINER_H
