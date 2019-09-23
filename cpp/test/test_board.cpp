#include <iostream>
#include <fstream>
#include <sstream>

#include "board.h"


int main() {
    std::ifstream infile;
    infile.open("conf/configure.txt");
    std::stringstream strStream;
    strStream << infile.rdbuf();
    infile.close();
    std::string str = strStream.str();

    renju::Configure conf;
    google::protobuf::TextFormat::ParseFromString(str, &conf);
    Board board = Board(conf.game_conf());
    board.print();
    board.step(Stone(1, 1, BLACK));
    board.print();
    board.step(Stone(1, 2, WHITE));
    board.step(Stone(2, 1, BLACK));
    board.step(Stone(2, 2, WHITE));
    board.step(Stone(3, 1, BLACK));
    board.step(Stone(3, 2, WHITE));
    std::tuple<bool, Player> res = board.step(Stone(4, 1, BLACK));
    board.print();
    std::cout << std::get<0>(res) << ":" << std::get<0>(res) << std::endl;
    return 0;
}