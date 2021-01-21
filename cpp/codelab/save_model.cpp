//
// Created by 刘也宽 on 2021/1/19.
//
#include "tensorflow/cc/saved_model/loader.h"
#include "tensorflow/cc/ops/io_ops.h"
#include <iostream>

using tensorflow::ops::SaveV2;
int main() {
    tensorflow::SavedModelBundle bundle;
    tensorflow::SessionOptions session_options;
    tensorflow::RunOptions run_options;
    LoadSavedModel(session_options, run_options, "/home/ubuntu/AlphaZero-Renju/cpp/model", {"serve"}, &bundle);
//    const auto &train_signature_def = bundle.GetSignatures().at("train_step");
//    auto train_x_name = train_signature_def.inputs().at("x").name();
//    std::cout << train_x_name << std::endl;
//    for (auto i = 0; i < bundle.meta_graph_def.graph_def().node_size(); i++) {
//        std::cout << bundle.meta_graph_def.graph_def().node(i).name() << std::endl;
//    }
    bundle.meta_graph_def.saver_def().PrintDebugString();
    tensorflow::Tensor checkpointPathTensor(tensorflow::DT_STRING, tensorflow::TensorShape());
    checkpointPathTensor.scalar<std::string>()() = "/home/ubuntu/AlphaZero-Renju/cpp/saved_model";
    bundle.session->Run(
            {{bundle.meta_graph_def.saver_def().filename_tensor_name(), checkpointPathTensor}},
            {}, {bundle.meta_graph_def.saver_def().save_tensor_name()}, nullptr);
//    auto output = std::vector<tensorflow::Tensor>();
//    bundle.session->Run({}, {"Adam/dense_2/kernel/v/Read/ReadVariableOp"}, {}, &output);
//    auto p = output[0].flat<float>().data();
//    for (int i = 0; i < output[0].NumElements(); i++) {
//        std::cout << *p << std::endl;
//        p++;
//    }
    return 0;
}
