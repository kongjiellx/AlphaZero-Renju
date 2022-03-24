#ifndef ALPHAZERO_RENJU_RANDOM_EXPERT_H
#define ALPHAZERO_RENJU_RANDOM_EXPERT_H

#include "cpp/src/strategy/expert.h"
#include <tuple>
#include <random>

class RandomExpert: public Expert {
private:
    std::mt19937 rng;
public:
    RandomExpert();
    shared_ptr<tuple<vector<float>, float>> get_pv(const BOARD_STATUS &status, const Player player) override;
};

#endif //ALPHAZERO_RENJU_RANDOM_EXPERT_H
