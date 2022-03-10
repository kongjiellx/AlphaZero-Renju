#ifndef ALPHAZERO_RENJU_PROTOBUF_UTILS_H
#define ALPHAZERO_RENJU_PROTOBUF_UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "conf/conf.pb.h"
#include <google/protobuf/text_format.h>

conf::Conf load_conf(const std::string path);


#endif //ALPHAZERO_RENJU_PROTOBUF_UTILS_H
