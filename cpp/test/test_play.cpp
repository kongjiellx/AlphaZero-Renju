//
// Created by 刘也宽 on 2020/11/11.
//
#include "cpp/src/pit.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "cpp/src/strategy/random_strategy.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/resource_manager.h"
DEFINE_string(conf_path, "", "conf file path");
int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    Pit pit;

    Strategy *stg1 = new MctsStrategy(
            ResourceManager::instance().get_conf().mtcs_conf(),
            Player::O
            );
//    Strategy *stg2 = new RandomStrategy();
    Strategy *stg2 = new MctsStrategy(ResourceManager::instance().get_conf().mtcs_conf(), Player::X);
    auto ret = pit.play_a_game(stg1, stg2);
    LOG(INFO) << "winner: " << ret.winner;
    for (auto& record: ret.records) {
        LOG(INFO) << std::get<0>(record.point) << ":" << std::get<1>(record.point);
//        LOG(INFO) << record.status[3][3];
    }
    LOG(INFO) << "instance";
    for(auto& instance: game_result_to_instances(ret)) {
//        LOG(INFO) << instance.label_v;
        LOG(INFO) << instance.features[0][0][0] << instance.features[0][0][1] << instance.features[0][0][2];
    }
    LOG(INFO) << "instance";
    return 0;
}
