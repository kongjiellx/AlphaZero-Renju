//
// Created by 刘也宽 on 2020/11/11.
//
#include "cpp/src/pit.h"
#include "gflags/gflags.h"
#include "cpp/src/strategy/random_strategy.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/resource_manager.h"

DEFINE_string(conf_path, "", "conf file path");
int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    Pit pit;
    Strategy *stg2 = new RandomStrategy();
//    Strategy *stg2 = new RandomStrategy();

    Strategy *stg1 = new MctsStrategy(ResourceManager().instance().get_conf().mtcs_conf());
    pit.play_a_game(stg1, stg2);
    return 0;
}
