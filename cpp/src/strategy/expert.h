#ifndef ALPHAZERO_RENJU_EXPERT_H
#define ALPHAZERO_RENJU_EXPERT_H

#include "cpp/src/data_structure/data_structure.h"
#include <tuple>

class Expert {
public:
    virtual shared_ptr<tuple<vector<float>, float>> get_pv(const BOARD_STATUS &status, const Player player) = 0;
};

#endif //ALPHAZERO_RENJU_EXPERT_H
