#include "model_manager.h"

ModelManager &ModelManager::instance() {
    static ModelManager ins(ResourceManager::instance().get_conf().game_conf().board_size());
    return ins;
}

ModelManager::ModelManager(int board_size): train_model(board_size), predict_model(board_size) {}

void ModelManager::init() {
    predict_model.init(ResourceManager::instance().get_conf().model_conf().model_path());
    train_model.init(ResourceManager::instance().get_conf().model_conf().model_path());
}

shared_ptr<tuple<vector<float>, float>> ModelManager::predict(const FEATURE &feature, MODEL_TYPE model_type) {
    shared_ptr<tuple<vector<float>, float>> ret;
    if (model_type == MODEL_TYPE::PREDICT) {
        predict_model_mutex.ReaderLock();
        ret = predict_model.predict({std::move(feature)});
        predict_model_mutex.ReaderUnlock();
    } else {
        train_model_mutex.ReaderLock();
        ret = train_model.predict({std::move(feature)});
        train_model_mutex.ReaderUnlock();
    }
    return ret;
}

void ModelManager::reset_train_model() {
    predict_model_mutex.Lock();
    save_model(MODEL_TYPE::PREDICT);
    predict_model_mutex.Unlock();
    train_model_mutex.Lock();
    train_model.load(ResourceManager::instance().get_conf().model_conf().predict_model_weight_save_path());
    train_model_mutex.Unlock();
}

void ModelManager::update_predict_model() {
    train_model_mutex.Lock();
    save_model(MODEL_TYPE::TRAIN);
    train_model_mutex.Unlock();
    predict_model_mutex.Lock();
    predict_model.load(ResourceManager::instance().get_conf().model_conf().train_model_weight_save_path());
    predict_model_mutex.Unlock();
}

void ModelManager::train_on_batch(std::vector<Instance> &instances) {
    train_model_mutex.Lock();
    train_model.train(instances);
    train_model_mutex.Unlock();
}

void ModelManager::save_model(MODEL_TYPE model_type) {
    if (model_type == MODEL_TYPE::PREDICT) {
        predict_model.save(ResourceManager::instance().get_conf().model_conf().predict_model_weight_save_path());
        LOG(INFO) << "predict model saved!";
    } else if (model_type == MODEL_TYPE::TRAIN) {
        train_model.save(ResourceManager::instance().get_conf().model_conf().train_model_weight_save_path());
        LOG(INFO) << "train model saved!";
    }
}

void ModelManager::load_model(MODEL_TYPE model_type) {
    if (model_type == MODEL_TYPE::PREDICT) {
        predict_model.load(ResourceManager::instance().get_conf().model_conf().predict_model_weight_save_path());
        LOG(INFO) << "predict model loaded!";
    } else if (model_type == MODEL_TYPE::TRAIN) {
        train_model.load(ResourceManager::instance().get_conf().model_conf().train_model_weight_save_path());
        LOG(INFO) << "train model loaded!";
    }
}
