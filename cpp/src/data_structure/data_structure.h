//
// Created by liuyekuan on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_DATA_STRUCTURE_H
#define ALPHAZERO_RENJU_DATA_STRUCTURE_H

#include <vector>

typedef std::vector<std::vector<int>> BOARD_STATUS;
typedef std::vector<std::vector<std::vector<float>>> FEATURE;

enum Result {
    P1_WIN,
    P2_WIN,
    TIE
};

struct History {
    BOARD_STATUS status;
    std::tuple<int, int> point;
};

struct GameResult {
    Result result;
    std::vector<History> history;
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

#endif //ALPHAZERO_RENJU_DATA_STRUCTURE_H
