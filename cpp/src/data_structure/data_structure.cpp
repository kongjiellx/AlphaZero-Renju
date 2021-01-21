//
// Created by 刘也宽 on 2020/8/14.
//
#include "data_structure.h"
#include "glog/logging.h"

shared_ptr<FEATURE> board_status_to_feature(const BOARD_STATUS &status, Player player) {
    auto features = make_shared<FEATURE>(status.size() * status.size() * 3, 0);
    for (auto i = 0; i < status.size(); i++) {
        for (auto j = 0; j < status.size(); j++) {
            if (status[i][j] == Player::O) {
                (*features)[(i * status.size() + j) * 3 + 0] = 1.0;
            } else if (status[i][j] == Player::X) {
                (*features)[(i * status.size() + j) * 3 + 1] = 1.0;
            }
            if (player == Player::O) {
                (*features)[(i * status.size() + j) * 3 + 2] = 1.0;
            }
        }
    }
    return features;
}

shared_ptr<std::vector<Instance>> game_result_to_instances(shared_ptr<GameResult> game_result) {
    auto instances = make_shared<std::vector<Instance>>();
    for (auto &record: game_result->records) {
        Instance instance;
        instance.label_p = std::move(record.distribution);
        instance.label_v = game_result->winner;
        instance.features = std::move(*board_status_to_feature(record.status, record.current_player));
        instances->push_back(std::move(instance));
    }
    return instances;
}
