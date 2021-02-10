#include "pit.h"
#include "board.h"
#include "cpp/src/resource_manager.h"
#include "glog/logging.h"

shared_ptr<GameResult> Pit::play_a_game(shared_ptr<Strategy> p1, shared_ptr<Strategy> p2, bool print) {
    auto ret = make_shared<GameResult>();
    Board board(ResourceManager::instance().get_conf().game_conf());
    auto current_stg = p1;
    while (true) {
        StepRecord step_record;
        step_record.status = board.get_current_status();
        auto position = current_stg->step(board, step_record);
        step_record.point = position;
        step_record.current_player = current_stg->getPlayer();

        DLOG(INFO) << step_record.current_player;
        DLOG(INFO) << std::get<0>(step_record.point) << ":" << std::get<1>(step_record.point);
        string debug_status = "====status====\n";
        for (auto &row: step_record.status) {
            for (auto &ii: row) {
                debug_status += std::to_string(ii) + " ";
            }
            debug_status += "\n";
        }
        DLOG(INFO) << debug_status;

        string distribution = "====distribution====\n";
        for (int i = 0; i < step_record.distribution.size(); i++) {
            distribution += std::to_string(step_record.distribution[i]) + " ";
            if (i % ResourceManager::instance().get_conf().game_conf().board_size() == 9) {
                distribution += "\n";
            }
        }
        DLOG(INFO) << distribution;

        ret->records.push_back(std::move(step_record));
        auto status = board.step(
                Stone(std::get<0>(position), std::get<1>(position), board.current_player));

        LOG_IF(INFO, print) << "======board======\n" + board.to_str();
        p1->post_process(board);
        p2->post_process(board);
        if (std::get<0>(status)) {
            ret->winner = std::get<1>(status);
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
