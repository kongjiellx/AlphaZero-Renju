#include <iostream>
#include <fstream>
#include <sstream>

#include "cpp/src/board.h"


int main() {
    std::ifstream infile;
    infile.open("conf/conf.pbtxt");
    std::stringstream strStream;
    strStream << infile.rdbuf();
    infile.close();
    std::string str = strStream.str();

    conf::Conf conf;
    google::protobuf::TextFormat::ParseFromString(str, &conf);
    Board board = Board(conf.game_conf());
    board.print();
    board.step(Stone(1, 1, O));
    board.print();
    board.step(Stone(1, 2, X));
    board.step(Stone(2, 1, O));
    board.step(Stone(2, 2, X));
    board.step(Stone(3, 1, O));
    board.step(Stone(3, 2, X));
    std::tuple<bool, Player> res = board.step(Stone(4, 1, O));
    board.print();
    std::cout << std::get<0>(res) << ":" << std::get<0>(res) << std::endl;
    return 0;
}