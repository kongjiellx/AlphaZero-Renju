//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_DATA_STRUCTURE_H
#define ALPHAZERO_RENJU_DATA_STRUCTURE_H

#include <vector>

typedef std::vector<std::vector<int>> BOARD_STATUS;
typedef std::vector<std::vector<std::vector<float>>> FEATURE;

enum Player {
    O = 1,
    X = -1,
    NONE = 0
};

struct StepRecord {
    BOARD_STATUS status;
    std::vector<float> distribution;
    std::tuple<int, int> point;
    Player current_player;
};

struct GameResult {
    Player winner;
    std::vector<StepRecord> records;
};

struct Instance {
    FEATURE features;
    std::vector<float> label_p;
    float label_v;
    Instance(int board_size)
    :features(board_size, std::vector<std::vector<float>>(board_size, std::vector<float>(3, 0))) {}
};

enum MODEL_TYPE {
    TRAIN,
    PREDICT
};

std::vector<Instance> game_result_to_instances(GameResult& game_result);

#endif //ALPHAZERO_RENJU_DATA_STRUCTURE_H
