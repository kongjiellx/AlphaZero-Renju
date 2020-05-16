//
// Created by 刘也宽 on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_PROTOBUF_UTILS_H
#define ALPHAZERO_RENJU_PROTOBUF_UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "conf/conf.pb.h"
#include <google/protobuf/text_format.h>

conf::Conf load_conf(const std::string path) {
    std::ifstream infile;
    infile.open(path);
    std::stringstream strStream;
    strStream << infile.rdbuf();
    infile.close();
    std::string str = strStream.str();

    conf::Conf conf;
    google::protobuf::TextFormat::ParseFromString(str, &conf);
    return conf;
}
#endif //ALPHAZERO_RENJU_PROTOBUF_UTILS_H
