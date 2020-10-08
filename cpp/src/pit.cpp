#include "pit.h"
#include "board.h"
#include "cpp/src/resource_manager.h"

GameResult Pit::play_a_game(const Strategy &p1, const Strategy &p2) {
    GameResult ret;
    Board board(ResourceManager::instance().get_conf().game_conf());
    bool done = false;
    while (!done) {
        StepRecord step_record;
        step_record.status = board.get_current_status();
        auto position = p1.step();
        step_record.point = position;
        ret.records.push_back(step_record);

        auto status = board.step(
                Stone(std::get<0>(position), std::get<1>(position), board.current_player));

        if (std::get<0>(status)) {
            ret.winner = std::get<1>(status);
            break;
        }
    }
    return ret;
}
