//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_ENGINE_H
#define ALPHAZERO_RENJU_ENGINE_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/producer.h"
#include "cpp/src/trainer.h"
#include "cpp/src/examiner.h"
#include <vector>


class Engine {
private:
    std::vector<Thread *> workers;
public:
    Engine();

    void add_workers();

    void start();

    void stop();
};

#endif //ALPHAZERO_RENJU_ENGINE_H
