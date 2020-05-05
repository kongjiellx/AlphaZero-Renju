//
// Created by liuyekuan on 19-10-20.
//

#ifndef ALPHAZERO_RENJU_MODEL_H
#define ALPHAZERO_RENJU_MODEL_H

#include <string>
#include <iostream>
#include <vector>
#include "tensorflow/cc/saved_model/loader.h"

class Model {
private:
    tensorflow::SavedModelBundle bundle;
public:
    Model() {}

    void train(tensorflow::Tensor x_data, tensorflow::Tensor p_data, tensorflow::Tensor v_data) {
        auto sigs = bundle.GetSignatures();
        const auto &signature_def = bundle.GetSignatures().at("train_step");
        const std::string x_name = signature_def.inputs().at("x").name();
        const std::string p_name = signature_def.inputs().at("p").name();
        const std::string v_name = signature_def.inputs().at("v").name();
        const std::string output_name = signature_def.outputs().at("output_0").name();

        std::vector<tensorflow::Tensor> outputs;
        bundle.session->Run({{x_name, x_data}, {p_name, p_data}, {v_name, v_data}}, {output_name}, {}, &outputs);
    }
    void predict(std::vector<float> data) {}

    void load(std::string export_dir) {
        ::tensorflow::SessionOptions session_options;
        ::tensorflow::RunOptions run_options;
        LoadSavedModel(session_options, run_options, export_dir, {"serve"}, &bundle);
    }

    void save(std::string path) {}
};

#endif //ALPHAZERO_RENJU_MODEL_H