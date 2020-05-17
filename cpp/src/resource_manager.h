//
// Created by 刘也宽 on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_RESOURCE_MANAGER_H
#define ALPHAZERO_RENJU_RESOURCE_MANAGER_H

#include "cpp/src/utils/fixed_deque.h"
#include "cpp/src/model.h"
#include "conf/conf.pb.h"
#include "cpp/src/utils/protobuf_utils.h"
#include "gflags/gflags.h"
#include "cpp/src/data_structure/data_structure.h"

DECLARE_bool(conf_path);

class ResourceManager {
private:
    FixedDeque<Instance> data_pool;
    conf::Conf conf;
public:
    ResourceManager() {
        data_pool = FixedDeque<Instance>(10000);
        conf = load_conf(FLAGS_conf_path);
    }

    static ResourceManager& instance() {
        static ResourceManager ins;
        return ins;
    }

    FixedDeque<Instance>& get_data_pool() {
        return data_pool;
    }

    const conf::Conf& get_conf() {
        return conf;
    }
};

#endif //ALPHAZERO_RENJU_RESOURCE_MANAGER_H
