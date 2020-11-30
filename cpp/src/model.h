//
// Created by liuyekuan on 19-10-20.
//

#ifndef ALPHAZERO_RENJU_MODEL_H
#define ALPHAZERO_RENJU_MODEL_H

#include <string>
#include <iostream>
#include <vector>
#include "tensorflow/cc/saved_model/loader.h"
#include "conf/conf.pb.h"
#include "cpp/src/data_structure/data_structure.h"
//#include "conf/conf_cc_proto_pb/conf/conf.pb.h"

class Model {
private:
    tensorflow::SavedModelBundle bundle;
    std::string train_x_name;
    std::string train_p_name;
    std::string train_v_name;
    std::string train_loss_name;
    std::string predict_x_name;
    int board_size;
public:
    Model(int board_size);

    float train(std::vector<float>& x_data, std::vector<float>& p_data, std::vector<float>& v_data);
    float train(std::vector<Instance> instances);
    void predict(std::vector<float> data);
    void load(std::string export_dir);
    void save(std::string path);
};

#endif //ALPHAZERO_RENJU_MODEL_H