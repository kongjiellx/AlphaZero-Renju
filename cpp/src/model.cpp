//
// Created by 刘也宽 on 2020/8/14.
//

#include "model.h"


Model::Model(int board_size): board_size(board_size) {}

float Model::train(std::vector<float>& x_data, std::vector<float>& p_data, std::vector<float>& v_data) {
    int batch_size = v_data.size();
    tensorflow::Tensor x_tensor(tensorflow::DataTypeToEnum<float>::v(),
                                 tensorflow::TensorShape{batch_size, board_size, board_size, 3});
    std::copy_n(x_data.begin(), x_data.size(), x_tensor.flat<float>().data());
    tensorflow::Tensor p_tensor(tensorflow::DataTypeToEnum<float>::v(),
                                 tensorflow::TensorShape{batch_size, board_size * board_size});
    std::copy_n(p_data.begin(), p_data.size(), p_tensor.flat<float>().data());
    tensorflow::Tensor v_tensor(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{batch_size});
    std::copy_n(v_data.begin(), v_data.size(), v_tensor.flat<float>().data());
    std::vector<tensorflow::Tensor> outputs;
    bundle.session -> Run({{train_x_name, x_tensor},
                         {train_p_name, p_tensor},
                         {train_v_name, v_tensor}},
                        {train_loss_name}, {}, &outputs);
    LOG(INFO) << "avg loss: " << outputs[0].scalar<float>().data()[0] / batch_size;
    return outputs[0].scalar<float>().data()[0];
}

float Model::train(std::vector<Instance>& instances) {
    std::vector<float> x_data;
    std::vector<float> p_data;
    std::vector<float> v_data;
    for (auto& instance: instances) {
        for (auto& row: instance.features) {
            for (auto& col: row) {
                x_data.insert(x_data.end(), col.begin(), col.end());
            }
        }
        p_data.insert(p_data.end(), instance.label_p.begin(), instance.label_p.end());
        v_data.push_back(instance.label_v);
    }
    return train(x_data, p_data, v_data);
}

void Model::predict(std::vector<float> data) {}

void Model::load(std::string export_dir) {
    ::tensorflow::SessionOptions session_options;
    ::tensorflow::RunOptions run_options;
    LoadSavedModel(session_options, run_options, export_dir, {"serve"}, &bundle);
    const auto& train_signature_def = bundle.GetSignatures().at("train_step");
    train_x_name = train_signature_def.inputs().at("x").name();
    train_p_name = train_signature_def.inputs().at("p").name();
    train_v_name = train_signature_def.inputs().at("v").name();
    train_loss_name = train_signature_def.outputs().at("output_0").name();

    const auto& predict_signature_def = bundle.GetSignatures().at("predict_func");
    predict_x_name = predict_signature_def.inputs().at("x").name();
    LOG(INFO) << "model loaded!";
}

void Model::save(std::string path) {

}


