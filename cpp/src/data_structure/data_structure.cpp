//
// Created by 刘也宽 on 2020/8/14.
//
#include "data_structure.h"

const FEATURE& board_status_to_feature(const BOARD_STATUS& status, Player player) {
    FEATURE features(status.size(), std::vector<std::vector<float>>(status.size(), std::vector<float>(3, 0)));
    for (auto i = 0; i < status.size(); i++) {
        for (auto j = 0; j < status.size(); j++) {
            if (status[i][j] == Player::O) {
                features[i][j][0] = 1;
            } else if (status[i][j] == Player::X) {
                features[i][j][1] = 1;
            }
            if (player == Player::O) {
                features[i][j][2] = 1;
            }
        }
    }
    return features;
}

const std::vector<Instance> game_result_to_instances(GameResult &game_result) {
    std::vector<Instance> instances;
    for (auto& record: game_result.records) {
        Instance instance;
        instance.label_p = record.distribution;
        instance.label_v = game_result.winner;
        instance.features = std::move(board_status_to_feature(record.status, record.current_player));
        instances.push_back(std::move(instance));
    }
    return instances;
}
