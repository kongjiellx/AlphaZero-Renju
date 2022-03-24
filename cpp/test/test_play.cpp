//
// Created by 刘也宽 on 2020/11/11.
//
#include "cpp/src/pit.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "cpp/src/strategy/random_strategy.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/model_manager.h"

DEFINE_string(conf_path,"", "conf file path");

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;

    ResourceManager::instance();
    ModelManager::instance().init();
    Pit pit;

    auto stg1 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::O, MODEL_TYPE::PREDICT);
    auto stg2 = make_shared<MctsStrategy>(ResourceManager::instance().get_conf().mtcs_conf(), Player::X, MODEL_TYPE::PREDICT);
    auto ret = pit.play_a_game(stg1, stg2, true);
    LOG(INFO) << "winner: " << ret->winner;

    auto p = game_result_to_instances(ret);
    return 0;
}
