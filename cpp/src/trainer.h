//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_TRAINER_H
#define ALPHAZERO_RENJU_TRAINER_H

#include "cpp/utils/thread.h"
#include "cpp/src/resource_manager.h"

class Trainer: Thread {
    void run() {
        Model& model = ResourceManager::instance().get_train_model();

    }
};


#endif //ALPHAZERO_RENJU_TRAINER_H
