#include "engine.h"
#include "gflags/gflags.h"
#include "spdlog/spdlog.h"

DEFINE_string(conf_path, "", "conf file path");

void signal_callback_handler(int signum) {
    // ModelManager::instance().save_model(MODEL_TYPE::TRAIN);
    // ModelManager::instance().save_model(MODEL_TYPE::PREDICT);
    exit(signum);
}

int main(int argc, char *argv[]) {
    spdlog::info("AlphaZero start.");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    Engine m(FLAGS_conf_path);
    m.add_workers();

    signal(SIGINT, signal_callback_handler);

    m.start();

    return EXIT_SUCCESS;
}