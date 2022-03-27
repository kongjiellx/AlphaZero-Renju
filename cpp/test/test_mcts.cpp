
#include "cpp/src/strategy/random_strategy.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/utils/protobuf_utils.h"
#include "cpp/src/strategy/random_expert.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    auto conf = load_conf("conf/conf.pbtxt");

    auto p1 = make_shared<MctsStrategy>(conf.mtcs_conf(), Player::O, make_shared<RandomExpert>(), true);
    auto p2 = make_shared<RandomStrategy>(Player::O);

    Board board(conf.game_conf());
    auto current_stg = (shared_ptr<Strategy>) p1;
    while (true) {
        StepRecord step_record;
        step_record.status = board.get_current_status();
        auto position = current_stg->step(board, step_record);
        step_record.point = position;
        step_record.current_player = current_stg->getPlayer();

        auto status = board.step(
                Stone(std::get<0>(position), std::get<1>(position), board.current_player));
        spdlog::info("======board======\n" + board.to_str());
        p1->post_process(board);
        p2->post_process(board);
        if (std::get<0>(status)) {
            break;
        }
        if (current_stg == p1) {
            current_stg = p2;
        } else {
            current_stg = p1;
        }
    }
    return 0;
}
