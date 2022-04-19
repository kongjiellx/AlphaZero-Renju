#ifndef ALPHAZERO_RENJU_PY_INTERFACE_H
#define ALPHAZERO_RENJU_PY_INTERFACE_H

#include "cpp/src/data_structure/data_structure.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/strategy/model_expert.h"

class PyInterface {
  private:
    Player player;
    std::shared_ptr<Board> board;
    std::shared_ptr<MctsStrategy> strategy;
  public:
    PyInterface();

    Player test_func(Player p);

    void init(std::string conf_path);

    void new_game(Player player);

    void enemy_move(std::tuple<int, int> pos);

    std::tuple<int, int> self_move();
};


#endif //ALPHAZERO_RENJU_PY_INTERFACE_H
