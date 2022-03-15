#ifndef ALPHAZERO_RENJU_EXPERTS_H
#define ALPHAZERO_RENJU_EXPERTS_H

#include "cpp/src/data_structure/data_structure.h"
#include <tuple>

class Expert {
public:
    virtual shared_ptr<tuple<vector<float>, float>> get_pv(const BOARD_STATUS &status, const Player player) = 0;
};

class ModelExpert: public Expert {
private:
    bool with_lock;
    MODEL_TYPE model_type;
public:
    ModelExpert(bool with_lock);
    shared_ptr<tuple<vector<float>, float>> get_pv(const BOARD_STATUS &status, const Player player) override;
};

#endif //ALPHAZERO_RENJU_EXPERTS_H
