#ifndef ALPHAZERO_RENJU_THREAD_H
#define ALPHAZERO_RENJU_THREAD_H

#include <thread>


class Thread {
public:
    Thread() : _running(false) {}

    virtual ~Thread() {}

    void start() {
        _running = true;
        _thread = std::thread([this]() {
            try {
                this->run();
            } catch (std::exception &e) {
                // LOG(ERROR) << "exception: " << e.what();
            } catch (...) {
                // LOG(ERROR) << "unknown exception";
            }
        });
    }

    void join() {
        if (_running) {
            _thread.join();
            _running = false;
        }
    }

    virtual void run() = 0;

    virtual void stop() = 0;

protected:
    bool _running;
    std::thread _thread;
};


#endif //ALPHAZERO_RENJU_THREAD_H
