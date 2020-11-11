#include "pit.h"
#include "board.h"
#include "cpp/src/resource_manager.h"

GameResult Pit::play_a_game(Strategy *p1, Strategy *p2) {
    GameResult ret;
    Board board(ResourceManager::instance().get_conf().game_conf());
    bool done = false;
    auto current_stg = p1;
    while (!done) {
        StepRecord step_record;
        step_record.status = board.get_current_status();
        auto position = current_stg->step(board);
        step_record.point = position;
        ret.records.push_back(step_record);

        auto status = board.step(
                Stone(std::get<0>(position), std::get<1>(position), board.current_player));
        board.print();
        if (std::get<0>(status)) {
            ret.winner = std::get<1>(status);
            break;
        }
        if (current_stg == p1) {
            current_stg = p2;
        } else {
            current_stg = p1;
        }
    }
    return ret;
}
