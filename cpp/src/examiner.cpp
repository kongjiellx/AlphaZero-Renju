#include "examiner.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/strategy/model_expert.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include <chrono>
#include "spdlog/spdlog.h"

Examiner::Examiner(int thread_num, int game_num): thread_pool(thread_num), game_num(game_num) {}

void Examiner::run() {
    spdlog::info("examiner start");
    Pit pit;
    int round = 0;
    while (true) {
        auto examiner_interval = ResourceManager::instance().get_conf().self_play_conf().examiner_interval();
        std::this_thread::sleep_for(std::chrono::minutes(examiner_interval));
        spdlog::info("game round: " + std::to_string(round) + " start!");
        ModelManager::instance().get_train_model_mutex().Lock();
        ModelManager::instance().get_predict_model_mutex().Lock();
        std::vector<std::future<shared_ptr<GameResult>>> futures;
        auto mcts_conf = ResourceManager::instance().get_conf().mcts_conf();
        for (size_t i = 0; i < game_num / 2; i++) {
            auto stg1 = make_shared<MctsStrategy>(mcts_conf, Player::O, make_shared<ModelExpert>(false, MODEL_TYPE::PREDICT), false);
            auto stg2 = make_shared<MctsStrategy>(mcts_conf, Player::X, make_shared<ModelExpert>(false, MODEL_TYPE::TRAIN), false);
            futures.emplace_back(thread_pool.enqueue(&Pit::play_a_game, &pit, stg1, stg2, false));
        }
        for (size_t i = 0; i < game_num / 2; i++) {
            auto stg1 = make_shared<MctsStrategy>(mcts_conf, Player::X, make_shared<ModelExpert>(false, MODEL_TYPE::PREDICT), false);
            auto stg2 = make_shared<MctsStrategy>(mcts_conf, Player::O, make_shared<ModelExpert>(false, MODEL_TYPE::TRAIN), false);
            futures.emplace_back(thread_pool.enqueue(&Pit::play_a_game, &pit, stg1, stg2, i == game_num / 2 - 1));
        }
        int new_win = 0, old_win = 0;
        int idx = 0;
        for (auto &&future: futures) {
            auto result = future.get();
            if ((idx < game_num / 2 && result->winner == Player::X) || (idx >= game_num / 2 && result->winner == Player::O)) {
                new_win++;
            } else if ((idx < game_num / 2 && result->winner == Player::O) || (idx >= game_num / 2 && result->winner == Player::X)) {
                old_win++;
            }
            idx++;
        }

        spdlog::info("new win: " + std::to_string(new_win) + ", old win: " + std::to_string(old_win));
        if (new_win > old_win) {
            spdlog::info("winner: NEW, update predict model!");
            ModelManager::instance().update_predict_model();
        } else {
            spdlog::info("winner: OLD, reset train model!");
            ModelManager::instance().reset_train_model();
        }
        ModelManager::instance().get_train_model_mutex().Unlock();
        ModelManager::instance().get_predict_model_mutex().Unlock();
        round++;
    }
}

void Examiner::stop() {}
