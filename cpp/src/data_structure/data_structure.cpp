//
// Created by 刘也宽 on 2020/8/14.
//
#include "data_structure.h"
#include "glog/logging.h"
#include <iostream>

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
        auto status = record.status;
        auto label_v = game_result->winner;
        auto label_p = record.distribution;
        auto player = record.current_player;

        Eigen::MatrixXi status_matrix(record.status.size(), record.status.size());
        for (int i = 0; i < record.status.size(); i++) {
            for (int j = 0; j < record.status.size(); j++) {
                status_matrix(i, j) = status[i][j];
            }
        }
        Eigen::MatrixXf label_matrix(record.status.size(), record.status.size());
        for (int i = 0; i < record.status.size(); i++) {
            for (int j = 0; j < record.status.size(); j++) {
                label_matrix(i, j) = label_p[i * record.status.size() + j];
            }
        }
//        std::cout << "====base_status====" << std::endl;
//        std::cout << status_matrix << std::endl;
//        std::cout << "====base_label====" << std::endl;
//        std::cout << label_matrix << std::endl;
        for (int i = 0; i < 4; i++) {
//            std::cout << "rotation: " << i << std::endl;
            auto rotated_status_matrix = rotation90(i, status_matrix);
//            std::cout << rotated_status_matrix << std::endl;
            auto rotated_label = rotation90(i, label_matrix);
//            std::cout << rotated_label << std::endl;
            BOARD_STATUS rotated_status(record.status.size(), vector<int>(record.status.size(), 0));
            for (int j = 0; j < record.status.size(); j++) {
                for (int k = 0; k < record.status.size(); k++) {
                    rotated_status[j][k] = *(rotated_status_matrix.data() + j * record.status.size() + k);
                }
            }

            auto reversed_status_matrix = rotated_status_matrix.rowwise().reverse().eval();
            auto reversed_label = rotated_label.rowwise().reverse().eval();
            BOARD_STATUS reversed_status(record.status.size(), vector<int>(record.status.size(), 0));
            for (int j = 0; j < record.status.size(); j++) {
                for (int k = 0; k < record.status.size(); k++) {
                    reversed_status[j][k] = *(reversed_status_matrix.data() + j * record.status.size() + k);
                }
            }

            Instance instance;
            instance.features = *board_status_to_feature(rotated_status, player);
            auto p = make_move_iterator(rotated_label.data());
            instance.label_p = vector<float>(p, p + rotated_label.size());
            instance.label_v = label_v;
            instances->push_back(std::move(instance));

            Instance instance2;
            instance2.features = *board_status_to_feature(reversed_status, player);
            auto p2 = make_move_iterator(reversed_label.data());
            instance2.label_p = vector<float>(p2, p2 + reversed_label.size());
            instance2.label_v = label_v;
            instances->push_back(std::move(instance2));
        }
    }
    return instances;
}

