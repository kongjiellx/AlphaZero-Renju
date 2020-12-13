//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_TRAINER_H
#define ALPHAZERO_RENJU_TRAINER_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/data_structure/data_structure.h"
#include <atomic>

class Trainer : public Thread {
private:
    std::atomic<int> total_consume_num;
public:
    void run() override;

    void stop() override;
};


#endif //ALPHAZERO_RENJU_TRAINER_H
