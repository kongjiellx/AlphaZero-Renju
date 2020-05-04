//
// Created by liuyekuan on 2020/5/3.
//
#include <string>
#include <iostream>
#include "tensorflow/cc/saved_model/loader.h"

int main() {
    const std::string export_dir = "/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1";
    ::tensorflow::SavedModelBundle bundle;
    ::tensorflow::SessionOptions session_options;
    ::tensorflow::RunOptions run_options;
    LoadSavedModel(session_options, run_options, export_dir, {"serve"},
                   &bundle);
    const auto& signature_def = bundle.GetSignatures().at("serving_default");
    const std::string input_name = signature_def.inputs().at("liuyekuan").name();
    const std::string output_name = signature_def.outputs().at("dense").name();

    std::vector<float> dx({1, 2});

    tensorflow::Tensor x_data(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{1, 2});
    std::copy_n(dx.begin(), dx.size(), x_data.flat<float>().data());
    LOG(INFO) << x_data.DebugString();
    std::vector<tensorflow::Tensor> outputs;
    bundle.session->Run({{input_name, x_data}}, {output_name}, {}, &outputs);
    std::cout << outputs[0].scalar<float>() << std::endl;
//    tensorflow::Session* session = bundle.GetSession();
    return 0;
}