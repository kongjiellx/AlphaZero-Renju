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
        Instance base;
        base.label_p = std::move(record.distribution);
        base.label_v = game_result->winner;
        base.features = std::move(*board_status_to_feature(record.status, record.current_player));

        Eigen::MatrixXf feature;
        std::copy_n(base.features.begin(), base.features.size(), feature.data());
        Eigen::MatrixXf label;
        std::copy_n(base.label_p.begin(), base.label_p.size(), label.data());
        for (int i = 0; i < 4; i++) {
            auto rotated_feature = rotation90(i, feature);
            auto rotated_label = rotation90(i, label);
            Instance instance;
            auto p = make_move_iterator(rotated_feature.data());
            instance.features = vector<float>(p, p + rotated_feature.size());
            p = make_move_iterator(rotated_label.data());
            instance.label_p = vector<float>(p, p + rotated_label.size());
            instance.label_v = base.label_v;
            instances->push_back(std::move(instance));

            auto reversed_feature = feature.rowwise().reverse().eval();
            auto reversed_label = label.rowwise().reverse().eval();
            Instance instance2;
            auto p2 = make_move_iterator(reversed_feature.data());
            instance2.features = vector<float>(p2, p2 + reversed_feature.size());
            p2 = make_move_iterator(reversed_label.data());
            instance2.label_p = vector<float>(p2, p2 + reversed_label.size());
            instance2.label_v = base.label_v;
            instances->push_back(std::move(instance2));
        }
    }
    return instances;
}

