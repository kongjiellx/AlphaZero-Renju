#include "resource_manager.h"
#include "spdlog/spdlog.h"

ResourceManager::ResourceManager() : conf(load_conf(FLAGS_conf_path)), data_pool(conf.self_play_conf().data_pool_size()) {}

ResourceManager &ResourceManager::instance() {
    static ResourceManager ins;
    return ins;
}

FixedDeque<Instance> &ResourceManager::get_data_pool() {
    return data_pool;
}

const conf::Conf &ResourceManager::get_conf() {
    return conf;
}
