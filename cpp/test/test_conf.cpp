#include <iostream>
#include <fstream>
#include <sstream>

#include "conf/conf.pb.h"
#include <google/protobuf/text_format.h>


int main() {
    std::ifstream infile;
    infile.open("conf/conf.pbtxt");
    std::stringstream strStream;
    strStream << infile.rdbuf();
    infile.close();
    std::string str = strStream.str();

    conf::Conf conf;
    google::protobuf::TextFormat::ParseFromString(str, &conf);
    std::string out_string;
    google::protobuf::TextFormat::PrintToString(conf, &out_string);
    std::cout << out_string << std::endl;
    return 0;
}