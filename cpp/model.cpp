//
// Created by liuyekuan on 19-10-20.
//

#include <vector>
#include "model.h"
#include "tf_utils.h"

void Model::create_graph() {
    x = Placeholder(scope, DT_FLOAT);
    y = Placeholder(scope, DT_FLOAT);

    m_vars["filter1"] = Variable(scope, {3, 3, 16, 3}, DT_FLOAT);
    m_vars["filter2"] = Variable(scope, {3, 3, 3, 1}, DT_FLOAT);
    m_vars["w1"] = Variable(scope, {64, 1}, DT_FLOAT);
    m_vars["b1"] = Variable(scope, {1, 1}, DT_FLOAT);

    layers["conv1"] = Conv2D(scope, x, m_vars["filter1"], {1, 3, 3, 1}, "SAME");
    layers["conv2"] = Conv2D(scope, x, m_vars["filter2"], {1, 3, 3, 1}, "VALID");
    layers["flatten"] = Reshape(scope, layers["conv2"], {-1, 64});
    layers["out"] = Tanh(scope, Add(scope, MatMul(scope, x, m_vars["w1"]), m_vars["b1"]));

    assigns.push_back(Assign(scope, m_vars["filter1"], TFUtils::XavierInit(scope, 3, 16, 3)));
    assigns.push_back(Assign(scope, m_vars["filter2"], TFUtils::XavierInit(scope, 3, 1, 3)));
    assigns.push_back(Assign(scope, m_vars["w1"], TFUtils::XavierInit(scope, 64, 1, 0)));
    assigns.push_back(Assign(scope, m_vars["b1"], RandomNormal(scope, {1, 1}, DT_FLOAT)));

    loss = ReduceMean(scope, Square(scope, Sub(scope, layers["out"], y)), {0, 1});

    std::vector<Output> weights_biases;
    for(std::pair<string, Output> i: m_vars)
        weights_biases.push_back(i.second);

    std::vector<Output> grad_outputs;
    TF_CHECK_OK(AddSymbolicGradients(scope, {loss}, weights_biases, &grad_outputs));
    for(int i = 0; i < weights_biases.size(); i++) {
        applies.push_back(ApplyGradientDescent(scope, weights_biases[i], Cast(scope, 0.01,  DT_FLOAT), {grad_outputs[i]}));
    }
}

void Model::train() {
    std::vector<Tensor> outputs;

    TF_CHECK_OK(session -> Run(assigns, nullptr));

    std::vector<float> dx({1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3});
    std::vector<float> dy({-1, 0, 1});

    Tensor x_data(DataTypeToEnum<float>::v(), TensorShape{10, 10, 3});
    std::copy_n(dx.begin(), dx.size(), x_data.flat<float>().data());
    LOG(INFO) << x_data.DebugString();

    Tensor y_data(DataTypeToEnum<float>::v(), TensorShape{3, 1});
    copy_n(dy.begin(), dy.size(), y_data.flat<float>().data());

    for(auto i = 0; i < 1000; i++) {
        TF_CHECK_OK(session -> Run({{x, x_data}, {y, y_data}}, {loss}, &outputs));
        std::cout << "Loss: " << outputs[0].scalar<float>() << std::endl;
        TF_CHECK_OK(session -> Run({{x, x_data}, {y, y_data}}, applies, nullptr));
    }
}

void Model::predict(std::vector<float> data) {
    Tensor x_data(DataTypeToEnum<float>::v(), TensorShape{3, 3});
    std::copy_n(data.begin(), data.size(), x_data.flat<float>().data());

    std::vector<Tensor> outputs;
    TF_CHECK_OK(session -> Run({{x, x_data}}, {layers["out"]}, &outputs));
    std::cout << "predict: " << outputs[0].matrix<float>() << std::endl;
}

void Model::load(string path) {

}

void Model::save(string path) {

}
