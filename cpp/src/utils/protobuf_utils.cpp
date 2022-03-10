#include "protobuf_utils.h"

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