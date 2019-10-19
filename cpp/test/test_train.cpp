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

    auto w1 = Variable(scope, {3, 1}, DT_FLOAT);
    auto assign_w1 = Assign(scope, w1, RandomNormal(scope, {3, 1}, DT_FLOAT));

    auto b1 = Variable(scope, {1, 1}, DT_FLOAT);
    auto assign_b1 = Assign(scope, b1, RandomNormal(scope, {1, 1}, DT_FLOAT));

    auto layer_1 = Tanh(scope, Add(scope, MatMul(scope, x, w1), b1));

    auto loss = ReduceMean(scope, Square(scope, Sub(scope, layer_1, y)), {0, 1});
    std::vector<Output> grad_outputs;
    TF_CHECK_OK(AddSymbolicGradients(scope, {loss}, {w1, b1}, &grad_outputs));
    auto apply_w1 = ApplyGradientDescent(scope, w1, Cast(scope, 0.01,  DT_FLOAT), {grad_outputs[0]});
    auto apply_b1 = ApplyGradientDescent(scope, b1, Cast(scope, 0.01,  DT_FLOAT), {grad_outputs[3]});

    ClientSession session(scope);
    std::vector<Tensor> outputs;

    TF_CHECK_OK(session.Run({assign_w1, assign_b1}, nullptr));


//    TF_CHECK_OK(session.Run({{x, x_data},
//                             {y, y_data}}, {loss}, &outputs));
//    std::cout << "Loss: " << outputs[0].scalar<float>() << std::endl;
    return 0;

}

