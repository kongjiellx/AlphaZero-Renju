//
// Created by liuyekuan on 19-10-20.
//

#include "model.h"
#include "tf_utils.h"

void Model::create_graph() {
    x = Placeholder(scope, DT_FLOAT);
    y = Placeholder(scope, DT_FLOAT);

    auto w1 = Variable(scope, {3, 1}, DT_FLOAT);
    assign_w1 = Assign(scope, w1, TFUtils::XavierInit(scope, 3, 1, 0));

    auto b1 = Variable(scope, {1, 1}, DT_FLOAT);
    assign_b1 = Assign(scope, b1, RandomNormal(scope, {1, 1}, DT_FLOAT));

    layer_1 = Tanh(scope, Add(scope, MatMul(scope, x, w1), b1));

    loss = ReduceMean(scope, Square(scope, Sub(scope, layer_1, y)), {0, 1});
    std::vector<Output> grad_outputs;
    TF_CHECK_OK(AddSymbolicGradients(scope, {loss}, {w1, b1}, &grad_outputs));
    apply_w1 = ApplyGradientDescent(scope, w1, Cast(scope, 0.01,  DT_FLOAT), {grad_outputs[0]});
    apply_b1 = ApplyGradientDescent(scope, b1, Cast(scope, 0.01,  DT_FLOAT), {grad_outputs[1]});
}

void Model::train() {
    std::vector<Tensor> outputs;

    TF_CHECK_OK(session -> Run({assign_w1, assign_b1}, nullptr));

    std::vector<float> dx({1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::vector<float> dy({-1, 0, 1});

    Tensor x_data(DataTypeToEnum<float>::v(), TensorShape{3, 3});
    std::copy_n(dx.begin(), dx.size(), x_data.flat<float>().data());
    LOG(INFO) << x_data.DebugString();

    Tensor y_data(DataTypeToEnum<float>::v(), TensorShape{3, 1});
    copy_n(dy.begin(), dy.size(), y_data.flat<float>().data());

    for(auto i = 0; i < 1000; i++) {
        TF_CHECK_OK(session -> Run({{x, x_data}, {y, y_data}}, {loss}, &outputs));
        std::cout << "Loss: " << outputs[0].scalar<float>() << std::endl;
        TF_CHECK_OK(session -> Run({{x, x_data}, {y, y_data}}, {apply_w1, apply_b1}, nullptr));
    }
}

void Model::predict(std::vector<float> data) {
    Tensor x_data(DataTypeToEnum<float>::v(), TensorShape{3, 3});
    std::copy_n(data.begin(), data.size(), x_data.flat<float>().data());

    std::vector<Tensor> outputs;
    TF_CHECK_OK(session -> Run({{x, x_data}}, {layer_1}, &outputs));
    std::cout << "predict: " << outputs[0].matrix<float>() << std::endl;
}

void Model::load(string path) {

}

void Model::save(string path) {

}
