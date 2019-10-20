//
// Created by liuyekuan on 19-10-20.
//

#ifndef CPP_MODEL_H
#define CPP_MODEL_H

#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/cc/framework/scope.h"
#include "tensorflow/cc/framework/gradients.h"
#include "tensorflow/cc/client/client_session.h"

using namespace tensorflow;
using namespace tensorflow::ops;

class Model {
private:
    Scope scope;
public:
    Model(): scope(Scope::NewRootScope()) {
    };
    void create_graph();
    void train();
    void predict();
    void load(string path);
    void save(string path);
};


#endif //CPP_MODEL_H
