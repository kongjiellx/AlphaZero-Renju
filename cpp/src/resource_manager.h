#ifndef ALPHAZERO_RENJU_RESOURCE_MANAGER_H
#define ALPHAZERO_RENJU_RESOURCE_MANAGER_H

#include "cpp/src/utils/fixed_deque.h"
#include "conf/conf.pb.h"
#include "cpp/src/utils/protobuf_utils.h"
#include "cpp/src/data_structure/data_structure.h"

class ResourceManager {
private:
    conf::Conf conf;
    FixedDeque<Instance> data_pool;

    ResourceManager(std::string conf_path);

public:
    static ResourceManager &instance(string conf_path="");

    FixedDeque<Instance> &get_data_pool();

    const conf::Conf &get_conf();
};

#endif //ALPHAZERO_RENJU_RESOURCE_MANAGER_H
