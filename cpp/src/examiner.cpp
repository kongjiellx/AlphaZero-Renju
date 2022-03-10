#include "examiner.h"
#include "cpp/src/pit.h"
#include "cpp/src/strategy/mcts_strategy.h"
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
        std::this_thread::sleep_for(std::chrono::minutes(5));
        spdlog::info("game round: " + std::to_string(round) + " start!");
        ModelManager::instance().get_train_model_mutex().Lock();
        ModelManager::instance().get_predict_model_mutex().Lock();
        std::vector<std::future<shared_ptr<GameResult>>> futures;
        auto mcts_conf = ResourceManager::instance().get_conf().mtcs_conf();
        // int old_simulate_num = mcts_conf.get_simulate_num();
        // mcts_conf.set_simulate_num(500);
        for (size_t i = 0; i < game_num; i++) {
            auto stg1 = make_shared<MctsStrategy>(mcts_conf, Player::O, MODEL_TYPE::TRAIN, false, false);
            auto stg2 = make_shared<MctsStrategy>(mcts_conf, Player::X, MODEL_TYPE::PREDICT, false, false);
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
        // mcts_conf.set_simulate_num(old_simulate_num);

        spdlog::info("O win: " + std::to_string(o_win) + ", X win: " + std::to_string(x_win));
        if (o_win > x_win) {
            spdlog::info("winner: O, update predict model!");
            ModelManager::instance().update_predict_model();
        } else {
            spdlog::info("winner: X, reset train model!");
            ModelManager::instance().reset_train_model();
        }
        ModelManager::instance().get_train_model_mutex().Unlock();
        ModelManager::instance().get_predict_model_mutex().Unlock();
        round++;
    }
}

void Examiner::stop() {}
