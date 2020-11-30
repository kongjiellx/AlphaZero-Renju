#include "model_manager.h"

ModelManager &ModelManager::instance() {
    static ModelManager ins;
    return ins;
}

void ModelManager::init() {
//    predict_model.load(ResourceManager::instance().get_conf().model_conf().model_path());
    train_model.load(ResourceManager::instance().get_conf().model_conf().model_path());
}

std::tuple<float, std::vector<float>> ModelManager::predict(FEATURE feature) {
    predict_model_mutex.ReaderLock();
    auto ret = std::tuple<float, std::vector<float>>(0.0, std::vector<float>{});
    predict_model_mutex.ReaderUnlock();
    return ret;
}

void ModelManager::reset_train_model() {
}

void ModelManager::update_predict_model() {}

void ModelManager::train_on_batch(std::vector<Instance> instances) {
    train_model.train(instances);
}
