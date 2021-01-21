//
// Created by 刘也宽 on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_MODEL_MANAGER_H
#define ALPHAZERO_RENJU_MODEL_MANAGER_H

#include "cpp/src/model.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/data_structure/data_structure.h"
#include "absl/synchronization/mutex.h"

class ModelManager {
private:
    absl::Mutex train_model_mutex;
    absl::Mutex predict_model_mutex;
    Model train_model;
//    Model predict_model;
public:
    static ModelManager &instance();

    ModelManager(int board_size);

    void init();

    void train_on_batch(std::vector<Instance> &instances);

    shared_ptr<tuple<vector<float>, float>> predict(const FEATURE &feature);

    void reset_train_model();

    void update_predict_model();

    void save_model(MODEL_TYPE model_type);
};


#endif //ALPHAZERO_RENJU_MODEL_MANAGER_H
