#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/cc/framework/scope.h"
#include "tensorflow/cc/framework/gradients.h"
#include "tensorflow/cc/client/client_session.h"

using namespace tensorflow;
using namespace tensorflow::ops;

Input XavierInit(Scope scope, int in_chan, int out_chan, int filter_side)
{
    float std;
    Tensor t;
    if(filter_side == 0)
    { //Dense
        std = sqrt(6.f/(in_chan+out_chan));
        Tensor ts(DT_INT64, {2});
        auto v = ts.vec<int64>();
        v(0) = in_chan;
        v(1) = out_chan;
        t = ts;
    }
    else
    { //Conv
        std = sqrt(6.f/(filter_side*filter_side*(in_chan+out_chan)));
        Tensor ts(DT_INT64, {4});
        auto v = ts.vec<int64>();
        v(0) = filter_side;
        v(1) = filter_side;
        v(2) = in_chan;
        v(3) = out_chan;
        t = ts;
    }
    auto rand = RandomUniform(scope, t, DT_FLOAT);
    return Multiply(scope, Sub(scope, rand, 0.5f), std*2.f);
}

int main() {
    Scope scope = Scope::NewRootScope();
    auto x = Placeholder(scope, DT_FLOAT);
    auto y = Placeholder(scope, DT_FLOAT);

    // weights init
    auto w1 = Variable(scope, {3, 3}, DT_FLOAT);
    auto assign_w1 = Assign(scope, w1, RandomNormal(scope, {3, 3}, DT_FLOAT));

    auto w2 = Variable(scope, {3, 2}, DT_FLOAT);
    auto assign_w2 = Assign(scope, w2, RandomNormal(scope, {3, 2}, DT_FLOAT));

    auto w3 = Variable(scope, {2, 1}, DT_FLOAT);
    auto assign_w3 = Assign(scope, w3, RandomNormal(scope, {2, 1}, DT_FLOAT));

// bias init
    auto b1 = Variable(scope, {1, 3}, DT_FLOAT);
    auto assign_b1 = Assign(scope, b1, RandomNormal(scope, {1, 3}, DT_FLOAT));

    auto b2 = Variable(scope, {1, 2}, DT_FLOAT);
    auto assign_b2 = Assign(scope, b2, RandomNormal(scope, {1, 2}, DT_FLOAT));

    auto b3 = Variable(scope, {1, 1}, DT_FLOAT);
    auto assign_b3 = Assign(scope, b3, RandomNormal(scope, {1, 1}, DT_FLOAT));

    auto layer_1 = Tanh(scope, Add(scope, MatMul(scope, x, w1), b1));
    auto layer_2 = Tanh(scope, Add(scope, MatMul(scope, layer_1, w2), b2));
    auto layer_3 = Tanh(scope, Add(scope, MatMul(scope, layer_2, w3), b3));

    // regularization
    auto regularization = AddN(scope,
                               std::initializer_list<Input>{L2Loss(scope, w1),
                                                       L2Loss(scope, w2),
                                                       L2Loss(scope, w3)});
    auto loss = Add(scope,
                    ReduceMean(scope, Square(scope, Sub(scope, layer_3, y)), {0, 1}),
                    Mul(scope, Cast(scope, 0.01,  DT_FLOAT), regularization));
    // add the gradients operations to the graph
    std::vector<Output> grad_outputs;
    TF_CHECK_OK(AddSymbolicGradients(scope, {loss}, {w1, w2, w3, b1, b2, b3}, &grad_outputs));

    std::cout << "go" << std::endl;
//    Scope root = Scope::NewRootScope();
//    auto input = Placeholder(root, DT_INT32);
//    auto y = Placeholder(root, DT_INT32);
//
//    auto dense_w = Variable(root, {10, 1}, DT_FLOAT);
//    auto dense = MatMul(root, input, dense_w);
//
//    auto loss = Sum(root, Square(root, Sub(root, dense, y)), {1});
//
////    auto assign_w1 = Assign(root, conv_filter, XavierInit(root, 3, 3, 3));
////    auto assign_w2 = Assign(root, dense_w, RandomNormal(root, {10, 1}, DT_FLOAT));
//
//    std::cout << "no grad" << std::endl;
//    std::vector<Output> grad_outputs;
//    auto status = AddSymbolicGradients(root, {loss}, {dense_w}, &grad_outputs);
////    std::cout << status << std::endl;
//    std::cout << "grad" << std::endl;
////    auto apply_w1 = ApplyGradientDescent(root, conv_filter, Cast(root, 0.01,  DT_FLOAT), {grad_outputs[0]});
////    auto apply_w2 = ApplyGradientDescent(root, dense_w, Cast(root, 0.01,  DT_FLOAT), {grad_outputs[1]});
////
////    tensorflow::GraphDef def;
////    TF_CHECK_OK(root.ToGraphDef(&def));
////    std::cout << "graph" << std::endl;
////    ClientSession session(root);
////    session-.Run({assign_w1, assign_w2}, nullptr);
//    std::cout << "doweqweeqwene" << std::endl;
    return 0;

//    for (int i = 0; i < 5000; ++i) {
//        if (i % 100 == 0) {
//            TF_CHECK_OK(session.Run({{x, x_data}, {y, y_data}}, {loss}, &outputs));
//            std::cout << "Loss after " << i << " steps " << outputs[0].scalar<float>() << std::endl;
//        }
//        // nullptr because the output from the run is useless
//        TF_CHECK_OK(session.Run({{x, x_data}, {y, y_data}}, {apply_w1, apply_w2, apply_w3, apply_b1, apply_b2, apply_b3}, nullptr));
//    }
}

