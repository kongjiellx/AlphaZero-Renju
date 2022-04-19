#include "cpp/src/py_interface.h"
#include "cpp/src/resource_manager.h"
#include "cpp/src/model_manager.h"

PyInterface::PyInterface() {}

Player PyInterface::test_func(Player p) {
    std::cout << "C++ player: " << p << std::endl;
    return p;
}

void PyInterface::init(std::string conf_path) {
    ResourceManager::instance(conf_path);
    ModelManager::instance(
            ResourceManager::instance().get_conf().game_conf().board_size(), 
            ResourceManager::instance().get_conf().model_conf()
    );
    ModelManager::instance().load_model(MODEL_TYPE::TRAIN);
    ModelManager::instance().load_model(MODEL_TYPE::PREDICT);
}

void PyInterface::new_game(Player player) {
    this->player = player;
    board = std::make_shared<Board>(ResourceManager::instance().get_conf().game_conf());
    strategy = make_shared<MctsStrategy>(
        ResourceManager::instance().get_conf().mcts_conf(), 
        player, 
        make_shared<ModelExpert>(false, MODEL_TYPE::PREDICT),
        false
    );
}

void PyInterface::enemy_move(std::tuple<int, int> pos) {
    board->step(Stone(std::get<0>(pos), std::get<1>(pos), board->current_player));
    strategy->post_process(*board);
}

std::tuple<int, int> PyInterface::self_move() {
    StepRecord step_record;
    std::cout << board->to_str() << std::endl;
    auto pos = strategy->step(*board, step_record);
    board->step(Stone(std::get<0>(pos), std::get<1>(pos), board->current_player));
    strategy->post_process(*board);
    return pos;
}