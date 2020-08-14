//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_MAIN_H
#define ALPHAZERO_RENJU_MAIN_H

#include "cpp/src/resource_manager.h"
#include "cpp/src/utils/thread.h"
#include "cpp/src/producer.h"
#include "cpp/src/trainer.h"
#include "cpp/src/examiner.h"
#include "gflags/gflags.h"
#include <vector>

DEFINE_string(conf_path, "", "conf file path");

class Main {
private:
    std::vector<Thread> workers;
public:
    Main() {
        ResourceManager::instance();
    }

    void add_workers() {
        workers.push_back(Producer(10));
        workers.push_back(Trainer());
        workers.push_back(Examiner());
    }

    void start() {
        for (auto& thread: workers) {
            thread.start();
        }
        for (auto& thread: workers) {
            thread.join();
        }
    }

    void stop() {
        for (auto& thread: workers) {
            thread.stop();
        }
    }
};

#endif //ALPHAZERO_RENJU_MAIN_H
