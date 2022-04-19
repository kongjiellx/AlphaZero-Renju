#include "resource_manager.h"
#include "spdlog/spdlog.h"

ResourceManager::ResourceManager(std::string conf_path) : conf(load_conf(conf_path)), data_pool(conf.self_play_conf().data_pool_size()) {
    std::cout << "do construct1!" << std::endl;
}

ResourceManager &ResourceManager::instance(string conf_path) {
    static ResourceManager ins(conf_path); // Won't do construct actually except first time called.
    return ins;
}

FixedDeque<Instance> &ResourceManager::get_data_pool() {
    return data_pool;
}

const conf::Conf &ResourceManager::get_conf() {
    return conf;
}
