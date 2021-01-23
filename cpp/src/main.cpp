//
// Created by liuyekuan on 2020/5/5.
//

#include "engine.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_string(conf_path, "", "conf file path");

void signal_callback_handler(int signum) {
    ModelManager::instance().save_model(MODEL_TYPE::TRAIN);
    ModelManager::instance().save_model(MODEL_TYPE::PREDICT);
    exit(signum);
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;

    Engine m;
    m.add_workers();

    signal(SIGINT, signal_callback_handler);

    m.start();

    return EXIT_SUCCESS;
}