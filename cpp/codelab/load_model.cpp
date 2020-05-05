//
// Created by liuyekuan on 2020/5/3.
//
#include <string>
#include <iostream>
#include <vector>
#include "tensorflow/cc/saved_model/loader.h"

int main() {
    const std::string export_dir = "/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1";
    ::tensorflow::SavedModelBundle bundle;
    ::tensorflow::SessionOptions session_options;
    ::tensorflow::RunOptions run_options;
    LoadSavedModel(session_options, run_options, export_dir, {"serve"},
                   &bundle);
    auto sigs = bundle.GetSignatures();
    const auto& signature_def = bundle.GetSignatures().at("train_step");
    for (auto i = signature_def.outputs().begin();i!=signature_def.outputs().end();i++) {
        std::cout<<i->first<<std::endl;
    }
    const std::string x = signature_def.inputs().at("x").name();
    const std::string p = signature_def.inputs().at("p").name();
    const std::string v = signature_def.inputs().at("v").name();

    const std::string output_name = signature_def.outputs().at("output_0").name();

    std::vector<float> dx;
    for (int i = 0; i < 300; i++) {
        dx.push_back(1.0);
    }
    tensorflow::Tensor x_data(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{1, 10, 10, 3});
    std::copy_n(dx.begin(), dx.size(), x_data.flat<float>().data());

    std::vector<float> dp;
    for (int i = 0; i < 100; i++) {
        dp.push_back(0.0);
    }
    dp[5] = 1.0;
    tensorflow::Tensor p_data(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{1, 100});
    std::copy_n(dp.begin(), dp.size(), p_data.flat<float>().data());

    std::vector<float> dv{0.3};
    tensorflow::Tensor v_data(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{1});
    std::copy_n(dv.begin(), dv.size(), v_data.flat<float>().data());

    std::cout<<"1" <<std::endl;
    std::vector<tensorflow::Tensor> outputs;
    for (int i = 0; i < 100; i++) {
        bundle.session->Run({{x, x_data},
                             {p, p_data},
                             {v, v_data}}, {output_name}, {}, &outputs);
        std::cout << outputs[0].scalar<float>() << std::endl;
    }
    std::cout<<"2" <<std::endl;

//    tensorflow::Session* session = bundle.GetSession();
    return 0;
}