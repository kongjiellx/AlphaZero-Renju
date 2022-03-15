#include "experts.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/data_structure/data_structure.h"

ModelExpert::ModelExpert(bool with_lock): with_lock(with_lock) {}

shared_ptr<tuple<vector<float>, float>> ModelExpert::get_pv(const BOARD_STATUS &status, const Player player) {
    auto features = board_status_to_feature(status, player);
    return ModelManager::instance().predict(*features, model_type, with_lock);
}