//
// Created by 刘也宽 on 2020/8/14.
//

#include "model.h"
#include "spdlog/spdlog.h"


Model::Model(int board_size) : board_size(board_size) {}

float Model::train(const vector<float> &x_data, const vector<float> &p_data, const vector<float> &v_data) {
    int batch_size = v_data.size();
    tensorflow::Tensor x_tensor(tensorflow::DataTypeToEnum<float>::v(),
                                tensorflow::TensorShape{batch_size, board_size, board_size, 3});
    std::copy_n(make_move_iterator(x_data.begin()), x_data.size(), x_tensor.flat<float>().data());
    tensorflow::Tensor p_tensor(tensorflow::DataTypeToEnum<float>::v(),
                                tensorflow::TensorShape{batch_size, board_size * board_size});
    std::copy_n(make_move_iterator(p_data.begin()), p_data.size(), p_tensor.flat<float>().data());
    tensorflow::Tensor v_tensor(tensorflow::DataTypeToEnum<float>::v(), tensorflow::TensorShape{batch_size});
    std::copy_n(make_move_iterator(v_data.begin()), v_data.size(), v_tensor.flat<float>().data());
    std::vector<tensorflow::Tensor> outputs;
    bundle.session->Run({{train_x_name, std::move(x_tensor)},
                         {train_p_name, std::move(p_tensor)},
                         {train_v_name, std::move(v_tensor)}},
                        {train_loss_name}, {}, &outputs);
    return outputs[0].scalar<float>().data()[0];
}

float Model::train(const vector<Instance> &instances) {
    std::vector<float> x_data;
    std::vector<float> p_data;
    std::vector<float> v_data;
    for (auto &instance: instances) {
        x_data.insert(x_data.end(), make_move_iterator(instance.features.begin()), make_move_iterator(instance.features.end()));
        p_data.insert(p_data.end(), make_move_iterator(instance.label_p.begin()), make_move_iterator(instance.label_p.end()));
        v_data.push_back(instance.label_v);
    }
    return train(x_data, p_data, v_data);
}

shared_ptr<tuple<vector<float>, float>> Model::predict(const vector<float> &data) {
    int batch_size = data.size() / (board_size * board_size * 3);
    assert(batch_size == 1);
    tensorflow::Tensor x_tensor(tensorflow::DataTypeToEnum<float>::v(),
                                tensorflow::TensorShape{batch_size, board_size, board_size, 3});
    std::copy_n(make_move_iterator(data.begin()), data.size(), x_tensor.flat<float>().data());
    std::vector<tensorflow::Tensor> outputs;
    bundle.session->Run({{predict_x_name, std::move(x_tensor)}},
                        {predict_p_name, predict_v_name}, {}, &outputs);
    auto begin = make_move_iterator(outputs[0].flat<float>().data());
    std::vector<float> ps(begin, begin + board_size * board_size);
    return make_shared<std::tuple<std::vector<float>, float>>(std::move(ps), outputs[1].flat<float>().data()[0]);
}

void Model::init(string export_dir) {
    tensorflow::SessionOptions session_options;
    tensorflow::RunOptions run_options;
    LoadSavedModel(session_options, run_options, export_dir, {"serve"}, &bundle);
    const auto &train_signature_def = bundle.GetSignatures().at("train_step");
    train_x_name = train_signature_def.inputs().at("x").name();
    train_p_name = train_signature_def.inputs().at("p").name();
    train_v_name = train_signature_def.inputs().at("v").name();
    train_loss_name = train_signature_def.outputs().at("output_0").name();

    const auto &predict_signature_def = bundle.GetSignatures().at("predict_func");
    predict_x_name = predict_signature_def.inputs().at("x").name();
    predict_p_name = predict_signature_def.outputs().at("output_0").name();
    predict_v_name = predict_signature_def.outputs().at("output_1").name();
    spdlog::info("model inited!");
}

void Model::save(string path) {
    tensorflow::Tensor checkpointPathTensor(tensorflow::DT_STRING, tensorflow::TensorShape());
    checkpointPathTensor.scalar<tensorflow::tstring>()() = path + "/weights";
    bundle.session->Run({{bundle.meta_graph_def.saver_def().filename_tensor_name(), checkpointPathTensor}},
            {}, {bundle.meta_graph_def.saver_def().save_tensor_name()}, nullptr);
}

void Model::load(string path) {
    tensorflow::Tensor checkpointPathTensor(tensorflow::DT_STRING, tensorflow::TensorShape());
    checkpointPathTensor.scalar<tensorflow::tstring>()() = path + "/weights";
    bundle.session->Run({{bundle.meta_graph_def.saver_def().filename_tensor_name(), checkpointPathTensor}}
    , {}, {bundle.meta_graph_def.saver_def().restore_op_name()}, nullptr);
}


