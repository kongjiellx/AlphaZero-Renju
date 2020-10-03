//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_TRAINER_H
#define ALPHAZERO_RENJU_TRAINER_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/data_structure/data_structure.h"

class Trainer: public Thread {
public:
    void run();

    void stop();
};


#endif //ALPHAZERO_RENJU_TRAINER_H
