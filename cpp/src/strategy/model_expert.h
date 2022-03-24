#ifndef ALPHAZERO_RENJU_MODEL_EXPERT_H
#define ALPHAZERO_RENJU_MODEL_EXPERT_H

#include "expert.h"

class ModelExpert: public Expert {
private:
    bool with_lock;
    MODEL_TYPE model_type;
public:
    ModelExpert(bool with_lock, MODEL_TYPE model_type);
    shared_ptr<tuple<vector<float>, float>> get_pv(const BOARD_STATUS &status, const Player player) override;
};


#endif //ALPHAZERO_RENJU_MODEL_EXPERT_H
