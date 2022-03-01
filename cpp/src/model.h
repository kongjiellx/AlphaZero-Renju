//
// Created by liuyekuan on 19-10-20.
//

#ifndef ALPHAZERO_RENJU_MODEL_H
#define ALPHAZERO_RENJU_MODEL_H
#include "tensorflow/cc/saved_model/loader.h"

#include <string>
#include <iostream>
#include <vector>
#include "conf/conf.pb.h"
#include "cpp/src/data_structure/data_structure.h"

class Model {
private:
    tensorflow::SavedModelBundle bundle;
    string train_x_name;
    string train_p_name;
    string train_v_name;
    string train_loss_name;
    string predict_x_name;
    string predict_p_name;
    string predict_v_name;
    int board_size;
public:
    Model(int board_size);

    float train(const vector<float> &x_data, const vector<float> &p_data, const vector<float> &v_data);

    float train(const vector<Instance> &instances);

    shared_ptr<tuple<vector<float>, float>> predict(const vector<float> &data);

    void init(string export_dir);

    void save(string path);

    void load(string path);
};

#endif //ALPHAZERO_RENJU_MODEL_H