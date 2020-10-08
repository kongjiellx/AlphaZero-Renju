#ifndef CPP_BOARD_H
#define CPP_BOARD_H

#include <vector>
#include <tuple>
#include "conf/conf.pb.h"
#include <google/protobuf/text_format.h>
#include "cpp/src/data_structure/data_structure.h"

enum Player {
    O = 1,
    X = -1,
    NOONE = 0
};

class Stone {
public:
    int x;
    int y;
    Player player;

    Stone(int x, int y, Player player) {
        this -> x = x;
        this -> y = y;
        this -> player = player;
    }

};

class Board {
private:
    int size;
    int win_num;
    std::vector<int> illegal_idx;
    std::tuple<int, int> last_pos;
    BOARD_STATUS board_status;

    void do_turn();
    bool is_legal(Stone stone);
    void add_stone(Stone stone);
    std::tuple<bool, Player > check_done(Stone stone);
    int count_on_direction(Stone stone, int xdirection, int ydirection, int num);
public:
    Player current_player;
    BOARD_STATUS get_current_status();
    Board(const conf::GameConf& conf);
    std::tuple<bool, Player > step(Stone stone);
    void print();
};


#endif //CPP_BOARD_H
