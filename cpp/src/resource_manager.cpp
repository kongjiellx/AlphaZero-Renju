//
// Created by 刘也宽 on 2020/10/3.
//
#include "resource_manager.h"

ResourceManager::ResourceManager() {
    data_pool = FixedDeque<Instance>(10000);
    conf = load_conf(FLAGS_conf_path);
}

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
