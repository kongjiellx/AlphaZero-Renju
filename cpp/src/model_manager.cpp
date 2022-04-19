#include "model_manager.h"
#include "spdlog/spdlog.h"

ModelManager &ModelManager::instance(int board_size, const conf::ModelConf &model_conf) {
    static ModelManager ins(board_size, model_conf); // Won't do construct actually except first time called.
    return ins;
}

ModelManager::ModelManager(int board_size, const conf::ModelConf &model_conf): 
        train_model(board_size), predict_model(board_size), model_conf(model_conf) {
    predict_model.init(model_conf.model_path());
    train_model.init(model_conf.model_path());
}

shared_ptr<tuple<vector<float>, float>> ModelManager::predict(const FEATURE &feature, MODEL_TYPE model_type, bool with_lock) {
    shared_ptr<tuple<vector<float>, float>> ret;
    if (model_type == MODEL_TYPE::PREDICT) {
        if (with_lock) {
            predict_model_mutex.ReaderLock();
        }
        ret = predict_model.predict({std::move(feature)});
        if (with_lock) {
            predict_model_mutex.ReaderUnlock();
        }
    } else {
        if (with_lock) {
            train_model_mutex.ReaderLock();
        }
        ret = train_model.predict({std::move(feature)});
        if (with_lock) {
            train_model_mutex.ReaderUnlock();
        }
    }
    return ret;
}

void ModelManager::reset_train_model() { // only called in examiner, can be lock free
    save_model(MODEL_TYPE::PREDICT, false);
    train_model.load(model_conf.predict_model_weight_save_path());
}

void ModelManager::update_predict_model() { // only called in examiner, can be lock free
    save_model(MODEL_TYPE::TRAIN, false);
    predict_model.load(model_conf.train_model_weight_save_path());
}

void ModelManager::train_on_batch(std::vector<Instance> &instances) {
    train_model_mutex.Lock();
    train_model.train(instances);
    train_model_mutex.Unlock();
}

void ModelManager::save_model(MODEL_TYPE model_type, bool with_lock) {
    if (model_type == MODEL_TYPE::PREDICT) {
        if (with_lock) {
            predict_model_mutex.ReaderLock();
        }
        predict_model.save(model_conf.predict_model_weight_save_path());
        if (with_lock) {
            predict_model_mutex.ReaderUnlock();
        }
        spdlog::info("predict model saved!");
    } else if (model_type == MODEL_TYPE::TRAIN) {
        if (with_lock) {
            train_model_mutex.ReaderLock();
        }
        train_model.save(model_conf.train_model_weight_save_path());
        if (with_lock) {
            train_model_mutex.ReaderUnlock();
        }
        spdlog::info("train model saved!");
    }
}

void ModelManager::load_model(MODEL_TYPE model_type) {
    if (model_type == MODEL_TYPE::PREDICT) {
        predict_model_mutex.Lock();
        predict_model.load(model_conf.predict_model_weight_save_path());
        predict_model_mutex.Unlock();
        spdlog::info("predict model loaded!");
    } else if (model_type == MODEL_TYPE::TRAIN) {
        train_model_mutex.Lock();
        train_model.load(model_conf.train_model_weight_save_path());
        train_model_mutex.Unlock();
        spdlog::info("train model loaded!");
    }
}

absl::Mutex &ModelManager::get_train_model_mutex() {
    return train_model_mutex;
}

absl::Mutex &ModelManager::get_predict_model_mutex() {
    return predict_model_mutex;
}
