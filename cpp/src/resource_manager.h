//
// Created by 刘也宽 on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_RESOURCE_MANAGER_H
#define ALPHAZERO_RENJU_RESOURCE_MANAGER_H

#include "cpp/utils/fixed_deque.h"
#include "cpp/src/model.h"
#include "conf/conf.pb.h"
#include "cpp/utils/protobuf_utils.h"
#include "gflags/gflags.h"

DECLARE_bool(conf_path);

class ResourceManager {
private:
    FixedDeque<int> data_pool;
    Model train_model;
    Model predict_model;
    conf::Conf conf;
public:
    ResourceManager(): data_pool(10000) {
        conf = load_conf(FLAGS_conf_path);
        train_model.load(conf.model_conf().model_path());
        predict_model.load(conf.model_conf().model_path());
    }

    static ResourceManager& instance() {
        static ResourceManager ins;
        return ins;
    }
};

#endif //ALPHAZERO_RENJU_RESOURCE_MANAGER_H
