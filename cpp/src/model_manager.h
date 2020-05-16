//
// Created by 刘也宽 on 2020/5/16.
//

#ifndef ALPHAZERO_RENJU_MODEL_MANAGER_H
#define ALPHAZERO_RENJU_MODEL_MANAGER_H

#include "cpp/src/model.h"
#include "cpp/src/resource_manager.h"
#include "absl/synchronization/mutex.h"

class ModelManager {
private:
    absl::Mutex train_model_mutex;
    absl::Mutex predict_model_mutex;
    Model train_model;
    Model predict_model;
public:
    void init() {
        predict_model.load(ResourceManager::instance().conf.model_conf().model_path());

    }
};


#endif //ALPHAZERO_RENJU_MODEL_MANAGER_H
