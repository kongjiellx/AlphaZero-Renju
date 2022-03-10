#ifndef ALPHAZERO_RENJU_EXAMINER_H
#define ALPHAZERO_RENJU_EXAMINER_H

#include "cpp/src/utils/thread.h"
#include "cpp/src/utils/thread_pool.h"

class Examiner : public Thread {
private:
    int game_num;
    ThreadPool thread_pool;

public:
    Examiner(int thread_num, int game_num);

    void run() override;

    void stop() override;
};


#endif //ALPHAZERO_RENJU_EXAMINER_H
