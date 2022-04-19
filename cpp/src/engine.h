#ifndef ALPHAZERO_RENJU_ENGINE_H
#define ALPHAZERO_RENJU_ENGINE_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/producer.h"
#include "cpp/src/trainer.h"
#include "cpp/src/examiner.h"
#include <vector>
#include "gflags/gflags.h"

DECLARE_string(conf_path);


class Engine {
private:
    std::vector<shared_ptr<Thread>> workers;
public:
    Engine(std::string conf_path);

    void add_workers();

    void start();

    void stop();
};

#endif //ALPHAZERO_RENJU_ENGINE_H
