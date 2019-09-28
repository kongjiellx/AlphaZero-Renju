#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/cc/framework/scope.h"

class Model {
private:
    tensorflow::GraphDef graphDef;
public:
    Model() {
        graphDef = create_graph();
    };

    tensorflow::GraphDef create_graph() {
        tensorflow::Scope root = tensorflow::Scope::NewRootScope();
        tensorflow::ops::Placeholder(root, tensorflow::DT_INT32, tensorflow::PartialTensorShape());
        tensorflow::ops::Conv2D()
//        auto a = Const<float>(root, {{3, 2}, {-1, 0}});
//        auto x = Const(root.WithOpName("x"), {{1.f}, {1.f}});
//        auto y = MatMul(root.WithOpName("y"), a, x);
//        auto y2 = Square(root, y);
//        auto y2_sum = Sum(root, y2, 0);
//        auto y_norm = Sqrt(root, y2_sum);
//        Div(root.WithOpName("y_normalized"), y, y_norm);

        tensorflow::GraphDef def;
        TF_CHECK_OK(root.ToGraphDef(&def));
        return def;
    }
};