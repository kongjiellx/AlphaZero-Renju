
//#include "cpp/src/resource_manager.h"
#include "cpp/src/strategy/mcts_strategy.h"
#include "cpp/src/utils/protobuf_utils.h"
#include "cpp/src/strategy/random_expert.h"

int main(int argc, char *argv[]) {
    auto conf = load_conf("conf/conf.pbtxt");

    auto stg1 = make_shared<MctsStrategy>(conf.mtcs_conf(), Player::O, make_shared<RandomExpert>(), true);
    std::cout << "dsada" << std::endl;
    return 0;
}
