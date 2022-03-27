#include "trainer.h"
#include "spdlog/spdlog.h"

void Trainer::run() {
    spdlog::info("trainer started!");
    int batch_size = ResourceManager::instance().get_conf().model_conf().batch_size();
    auto train_step_interval = ResourceManager::instance().get_conf().self_play_conf().train_step_interval();
    auto trainer_log_freq = ResourceManager::instance().get_conf().self_play_conf().trainer_log_freq();
    int i = 0;
    while (true) {
        std::vector<Instance> instances;
        ResourceManager::instance().get_data_pool().sample_batch(instances, batch_size);
        ModelManager::instance().train_on_batch(instances);
        total_consume_num += instances.size();
        if (i++ % trainer_log_freq == 0) {
            spdlog::info("total_consume_num: " + std::to_string(total_consume_num));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(train_step_interval));
    }
}

void Trainer::stop() {}
