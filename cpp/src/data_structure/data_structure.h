//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_DATA_STRUCTURE_H
#define ALPHAZERO_RENJU_DATA_STRUCTURE_H

#include <vector>
#include <tuple>
#include <memory>

using std::make_shared;
using std::make_move_iterator;
using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using std::string;
using std::tuple;

typedef vector<vector<int>> BOARD_STATUS;
typedef vector<float> FEATURE;

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
};

enum MODEL_TYPE {
    TRAIN,
    PREDICT
};

shared_ptr<FEATURE> board_status_to_feature(const BOARD_STATUS &status, Player player);

shared_ptr<FEATURE> board_status_to_feature_with_augmentation(const BOARD_STATUS &status, Player player);

shared_ptr<std::vector<Instance>> game_result_to_instances(shared_ptr<GameResult> game_result);



#endif //ALPHAZERO_RENJU_DATA_STRUCTURE_H
