#include <iostream>
#include <fstream>
#include <sstream>

#include "tensorflow/core/protobuf/saved_model.pb.h"
#include <google/protobuf/text_format.h>


int main() {
    std::fstream infile("saved_model.pb", std::ios::in | std::ios::binary);
    tensorflow::SavedModel savedModel;
    savedModel.ParseFromIstream(&infile);
    infile.close();
    std::cout << savedModel.SerializeAsString() << std::endl;

    std::string out_string;
    google::protobuf::TextFormat::PrintToString(savedModel, &out_string);
    std::cout << "s" << ":" << out_string << std::endl;
    return 0;
}