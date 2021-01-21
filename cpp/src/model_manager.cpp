#include "model_manager.h"

ModelManager &ModelManager::instance() {
    static ModelManager ins(ResourceManager::instance().get_conf().game_conf().board_size());
    return ins;
}

ModelManager::ModelManager(int board_size) : train_model(board_size) {

}

void ModelManager::init() {
//    predict_model.load(ResourceManager::instance().get_conf().model_conf().model_path());
    train_model.init(ResourceManager::instance().get_conf().model_conf().model_path());
}

const std::tuple<std::vector<float>, float> ModelManager::predict(const FEATURE &feature) {
    return train_model.predict(feature);
}

void ModelManager::reset_train_model() {
}

void ModelManager::update_predict_model() {}

void ModelManager::train_on_batch(std::vector<Instance> &instances) {
    train_model.train(instances);
}

void ModelManager::save_model(MODEL_TYPE model_type) {
    if (model_type == MODEL_TYPE::PREDICT) {
    } else if (model_type == MODEL_TYPE::TRAIN) {
        train_model.save(ResourceManager::instance().get_conf().model_conf().train_model_weight_save_path());
        LOG(INFO) << "train model saved!";
    }
}