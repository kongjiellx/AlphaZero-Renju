#include "model_manager.h"

ModelManager &ModelManager::instance() {
    static ModelManager ins;
    return ins;
}

void ModelManager::init() {
    predict_model.load(ResourceManager::instance().get_conf().model_conf().model_path());
    train_model.load(ResourceManager::instance().get_conf().model_conf().model_path());
}

std::tuple<float, std::vector<float>> ModelManager::predict(FEATURE feature, MODEL_TYPE model_type) {
    return std::tuple<float, std::vector<float>>(0.0, std::vector<float>{});
}

void ModelManager::reset_train_model() {}

void ModelManager::update_predict_model() {}
