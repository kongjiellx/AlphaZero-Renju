//
// Created by liuyekuan on 19-10-20.
//

#include "model.h"
#include "tf_utils.h"

void Model::create_graph() {
    auto x = Placeholder(scope, DT_FLOAT);
    auto y = Placeholder(scope, DT_FLOAT);

    auto w1 = Variable(scope, {3, 3, 1, 1}, DT_FLOAT);
    auto assign_w1 = Assign(scope, w1, TFUtils::XavierInit(scope, 1, 1, 3));

    auto layer1 = Conv2D(scope, x, w1, {1, 1, 1, 1}, "VALID");
    auto layer2 = Reshape(scope, layer1, {-1, 1});

    auto loss = ReduceMean(scope, Square(scope, Sub(scope, layer2, y)), {0, 1});
    std::vector<Output> grad_outputs;
    TF_CHECK_OK(AddSymbolicGradients(scope, {loss}, {w1}, &grad_outputs));

    auto apply_w1 = ApplyGradientDescent(scope, w1, Cast(scope, 0.0001,  DT_FLOAT), {grad_outputs[0]});
}

void Model::train() {

}

void Model::predict() {

}

void Model::load(string path) {

}

void Model::save(string path) {

}
