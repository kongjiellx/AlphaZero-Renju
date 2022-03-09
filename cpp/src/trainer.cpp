#include "trainer.h"
#include "spdlog/spdlog.h"

void Trainer::run() {
    spdlog::info("trainer started!");
    int batch_size = ResourceManager::instance().get_conf().model_conf().batch_size();
    int i = 0;
    while (true) {
        std::vector<Instance> instances;
        ResourceManager::instance().get_data_pool().sample_batch(instances, batch_size);
        ModelManager::instance().train_on_batch(instances);
        total_consume_num += instances.size();
        if (i++ % 100 == 0) {
            spdlog::info("total_consume_num: " + std::to_string(total_consume_num));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void Trainer::stop() {}
