#ifndef ALPHAZERO_RENJU_RESOURCE_MANAGER_H
#define ALPHAZERO_RENJU_RESOURCE_MANAGER_H

#include "cpp/src/utils/fixed_deque.h"
#include "conf/conf.pb.h"
#include "cpp/src/utils/protobuf_utils.h"
#include "gflags/gflags.h"
#include "cpp/src/data_structure/data_structure.h"

DECLARE_string(conf_path);

class ResourceManager {
private:
    FixedDeque<Instance> data_pool;
    conf::Conf conf;

    ResourceManager();

public:
    static ResourceManager &instance();

    FixedDeque<Instance> &get_data_pool();

    const conf::Conf &get_conf();
};

#endif //ALPHAZERO_RENJU_RESOURCE_MANAGER_H
