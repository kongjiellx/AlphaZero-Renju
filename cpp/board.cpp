#include <iostream>
#include "board.h"

Board::Board(const renju::GameConf &conf) {
    this -> size = conf.board_size();
    this -> win_num = conf.win_num();
    this -> current_player = BLACK;
    for (int i = 0; i < size; i ++) {
        this -> board.emplace_back(size, 0);
    }
}

void Board::do_turn() {
    if (current_player == BLACK) {
        current_player = WHITE;
    } else {
        current_player = BLACK;
    }

}

std::vector<std::vector<std::vector<int>>> Board::get_feature() {
    return std::vector<std::vector<std::vector<int>>>();
}

bool Board::is_legal(Stone stone) {
    return stone.player == current_player and board[stone.x][stone.y] == 0;
}

void Board::add_stone(Stone stone) {
    board[stone.x][stone.y] = stone.player;
    last_pos = std::make_tuple(stone.x, stone.y);
    illegal_idx.push_back(stone.x * size + stone.y);
}

std::tuple<bool, Player> Board::check_done(Stone stone) {
    std::vector<std::vector<std::tuple<int, int>>> directions {
            {{-1, 0}, {1, 0}},
            {{0, -1}, {0, 1}},
            {{-1, 1}, {1, -1}},
            {{-1, -1}, {1, 1}}
    };
    for (auto& axis: directions) {
        int axis_count = 1;
        for (auto& xy: axis) {
            axis_count += count_on_direction(stone, std::get<0>(xy), std::get<1>(xy));
            if (axis_count >= win_num) {
                return std::make_tuple(true, stone.player);
            }
        }
    }
    return std::make_tuple(illegal_idx.size() >= size * size, NOONE);
}

std::tuple<bool, Player> Board::step(Stone stone) {
    if (!is_legal(stone)) {
        throw std::runtime_error("illegal idx");
    }
    add_stone(stone);
    do_turn();
    return check_done(stone);
}

int Board::count_on_direction(Stone stone, int xdirection, int ydirection) {
    int count = 0;
    for (int step = 1; step < 5; step ++) {
        if ((xdirection != 0) && (stone.y + xdirection * step < 0 || stone.y + xdirection * step >= size)) {
            break;
        }
        if ((ydirection != 0) && (stone.x + ydirection * step < 0 || stone.x + ydirection * step >= size)) {
            break;
        }
        if (board[stone.x + ydirection * step][stone.y + xdirection * step] == stone.player) {
            count ++;
        } else {
            break;
        }
    }
    return count;
}

void Board::print() {
    std::cout << "=======board=======" << std::endl;
    std::string str = "";
    for (int i = 0; i < size; i ++) {
        std::string row = "";
        for (int j = 0; j < size; j ++) {
            if (i == std::get<0>(last_pos) && j == std::get<1>(last_pos) && board[i][j] == BLACK) {
                row += "O)";
            } else if(i == std::get<0>(last_pos) && j == std::get<1>(last_pos) && board[i][j] == WHITE){
                row += "X)";
            } else if(board[i][j] == BLACK){
                row += "O ";
            } else if(board[i][j] == WHITE) {
                row += "X ";
            } else {
                row += "- ";
            }
        }
        str += (row + "\n");
    }
    std::cout << str;
}
