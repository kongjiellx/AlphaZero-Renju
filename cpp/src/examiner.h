//
// Created by liuyekuan on 2020/5/5.
//

#ifndef ALPHAZERO_RENJU_EXAMINER_H
#define ALPHAZERO_RENJU_EXAMINER_H

#include "cpp/src/utils/thread.h"


class Examiner: public Thread {
    void run() override;
    void stop() override;
};


#endif //ALPHAZERO_RENJU_EXAMINER_H
