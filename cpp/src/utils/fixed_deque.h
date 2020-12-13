//
// Created by 刘也宽 on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_FIXED_DEQUE_H
#define ALPHAZERO_RENJU_FIXED_DEQUE_H

#include <deque>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <random>
#include <exception>
#include "absl/synchronization/mutex.h"


template<class T>
class FixedDeque {
private:
    std::deque<T> _deque;
    absl::Mutex _mutex;
    absl::CondVar _can_push;
    absl::CondVar _can_pop;
    int size;
    std::uniform_int_distribution<std::mt19937::result_type> pool_d;
    std::mt19937 rng;
public:
    FixedDeque(int size) : size(size) {
        std::random_device dev;
        pool_d = std::uniform_int_distribution<std::mt19937::result_type>(0, size - 1);
        rng = std::mt19937(dev());
    }

    bool full() {
        return _deque.size() >= size;
    }

    void push_back(const T &e) {
        _mutex.Lock();
        if (_deque.size() >= size) {
            _deque.pop_front();
        }
        _deque.push_back(e);
        _can_pop.Signal();
        _mutex.Unlock();
    }

    T pop_front() {
        _mutex.Lock();
        while (_deque.empty()) {
            bool timeout = _can_pop.WaitWithTimeout(&_mutex, absl::Seconds(10));
            if (timeout) {
                _mutex.Unlock();
                throw std::runtime_error("timeout!");
            }
        }
        T e(_deque.front());
        _deque.pop_front();
        _can_push.Signal();
        _mutex.Unlock();
        return e;
    }

    int get_size() {
        return size;
    }

    int get_real_size() {
        return _deque.size();
    }

    T operator[](int pos) {
        _mutex.ReaderLock();
        T e = _deque[pos];
        _mutex.ReaderUnlock();
        return e;
    }

    std::vector<T> sample_batch(int batch_num) {
        std::vector<T> ret;
        _mutex.ReaderLock();
        for (int i = 0; i < batch_num; i++) {
            T t = _deque[pool_d(rng)];
            ret.push_back(t);
        }
        _mutex.ReaderUnlock();
        return ret;
    }
};


#endif //ALPHAZERO_RENJU_FIXED_DEQUE_H
