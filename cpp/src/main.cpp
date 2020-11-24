//
// Created by liuyekuan on 2020/5/5.
//

#include "engine.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_string(conf_path, "", "conf file path");

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;

    Engine m;
    m.add_workers();
    m.start();
    return 0;
}