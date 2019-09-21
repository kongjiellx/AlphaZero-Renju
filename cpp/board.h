#ifndef CPP_BOARD_H
#define CPP_BOARD_H

#include <vector>
#include <tuple>
#include "conf/configure.pb.h"
#include "conf/configure_cc_proto_pb/conf/configure.pb.h"
#include <google/protobuf/text_format.h>

enum Player {
    BLACK = 1,
    WHITE = -1,
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
    Player current_player;
    std::vector<int> illegal_idx;
    std::tuple<int, int> last_pos;
    std::vector<std::vector<int>> board;
public:
    Board(renju::GameConf& conf);
    void do_turn();
    std::vector<std::vector<std::vector<int>>> get_feature();
    bool is_legal(Stone stone);
    void add_stone(Stone stone);
    std::tuple<bool, Player > check_done(Stone stone);
    std::tuple<bool, Player > step(Stone stone);
    int count_on_direction(Stone stone, int xdirection, int ydirection);
};


#endif //CPP_BOARD_H
