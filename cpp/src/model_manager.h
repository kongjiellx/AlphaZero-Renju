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
    Model predict_model;
public:
    static ModelManager &instance();

    ModelManager(int board_size);

    void init();

    void train_on_batch(std::vector<Instance> &instances);

    shared_ptr<tuple<vector<float>, float>> predict(const FEATURE &feature, MODEL_TYPE model_type, bool with_lock=true);

    void reset_train_model();

    void update_predict_model();

    void save_model(MODEL_TYPE model_type, bool with_lock=true);

    void load_model(MODEL_TYPE model_type);

    absl::Mutex &get_train_model_mutex();

    absl::Mutex &get_predict_model_mutex();
};


#endif //ALPHAZERO_RENJU_MODEL_MANAGER_H
