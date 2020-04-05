//
// Created by liuyekuan on 19-10-20.
//

#ifndef CPP_TF_UTILS_H
#define CPP_TF_UTILS_H

#include "tensorflow/cc/framework/scope.h"

using namespace tensorflow;

class TFUtils {
public:
    static Input XavierInit(Scope scope, int in_chan, int out_chan, int filter_side);
};


#endif //CPP_TF_UTILS_H
