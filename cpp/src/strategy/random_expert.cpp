#include "random_expert.h"

RandomExpert::RandomExpert() {
    std::random_device dev;
    rng = std::mt19937(dev());
}

shared_ptr<tuple<vector<float>, float>> RandomExpert::get_pv(const BOARD_STATUS &status, const Player player) {
    int board_size = status.size();
    auto rand_g = std::uniform_real_distribution<float>(0, 1);
    vector<float> ps;
    for (int i = 0; i < board_size * board_size; i++) {
        ps.push_back(rand_g(rng));
    }
    auto v = rand_g(rng);
    return make_shared<std::tuple<vector<float>, float>>(ps, v);
}