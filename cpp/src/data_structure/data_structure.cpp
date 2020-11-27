//
// Created by 刘也宽 on 2020/8/14.
//
#include "data_structure.h"

std::vector<Instance> game_result_to_instances(GameResult &game_result) {
    std::vector<Instance> instances;
    for (auto& record: game_result.records) {
        Instance instance(record.status.size());
        instance.label_p = record.distribution;
        instance.label_v = game_result.winner;

        for (int i = 0; i < record.status.size(); i++) {
            for (int j = 0; j < record.status.size(); j++) {
                if (record.status[i][j] == Player::O) {
                    instance.features[i][j][0] = 1;
                } else if (record.status[i][j] == Player::X) {
                    instance.features[i][j][1] = 1;
                }
                if (record.current_player == Player::O) {
                    instance.features[i][j][2] = 1;
                }
            }
        }
        instances.push_back(std::move(instance));
    }
    return instances;
}
