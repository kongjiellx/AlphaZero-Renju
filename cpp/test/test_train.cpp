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
    std::cout << "go" << std::endl;
    Scope root = Scope::NewRootScope();
    auto input = Placeholder(root, DT_INT32, Placeholder::Shape({-1, 10, 10, 3}));
    auto y = Placeholder(root, DT_INT32, Placeholder::Shape({-1, 1}));
    auto conv_filter = Variable(root, {3, 3, 3, 16}, DT_FLOAT);
    auto conv = Conv2D(root, input, conv_filter, {1, 1}, "SAME");
    auto relu = Relu(root, conv);
    auto flatten = Reshape(root, relu, {-1, 10 * 10 * 16});
    auto dense_w = Variable(root, PartialTensorShape({10 * 10 * 16, 1}), DT_FLOAT);
    auto dense = MatMul(root, flatten, dense_w);

    auto loss = ReduceMean(root, Square(root, Sub(root, dense, y)), {0, 1});

    auto assign_w1 = Assign(root, conv_filter, XavierInit(root, 3, 16, 3));
    auto assign_w2 = Assign(root, dense_w, XavierInit(root, 10 * 10 * 16, 1, 0));

    std::cout << "no grad" << std::endl;
    std::vector<Output> grad_outputs;
    AddSymbolicGradients(root, {loss}, {conv_filter, dense_w}, &grad_outputs);
    std::cout << "grad" << std::endl;
    auto apply_w1 = ApplyGradientDescent(root, conv_filter, Cast(root, 0.01,  DT_FLOAT), {grad_outputs[0]});
    auto apply_w2 = ApplyGradientDescent(root, dense_w, Cast(root, 0.01,  DT_FLOAT), {grad_outputs[1]});

    tensorflow::GraphDef def;
    TF_CHECK_OK(root.ToGraphDef(&def));
    std::cout << "graph" << std::endl;
    ClientSession session(root);
    session.Run({assign_w1, assign_w2}, nullptr);
    std::cout << "done" << std::endl;
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

