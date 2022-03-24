#ifndef ALPHAZERO_RENJU_MCTS_STRATEGY_H
#define ALPHAZERO_RENJU_MCTS_STRATEGY_H

#include "strategy.h"
#include "conf/conf_cc_proto_pb/conf/conf.pb.h"
#include <tuple>
#include <unordered_map>
#include "cpp/src/strategy/expert.h"

class Node: public std::enable_shared_from_this<Node> {
private:
    weak_ptr<Node> parent;
    int N;
    float W;
    float P;
    Player player;
    std::unordered_map<int, shared_ptr<Node>> children;
public:
    Player getPlayer() const;

    void setParent(weak_ptr<Node> parent);

    std::unordered_map<int, shared_ptr<Node>> &getChildren();

    Node(weak_ptr<Node> parent, float p, Player player);

    float Q();

    int getN();

    float getP();

    tuple<shared_ptr<Node>, int> select();

    void expand(vector<float> ps, Player player);

    void backup(float v);

    bool is_leaf();
};

class MctsStrategy : public Strategy {
private:
    shared_ptr<Node> root;
    shared_ptr<Node> current_root;
    int current_step;
    conf::MctsConf mcts_conf;
    bool sample; // true: choice from distribution; false: choice max
    shared_ptr<Expert> expert;

    void dirichlet_noise(std::vector<float> &ps);

    void simulate(const Board &board, bool add_dirichlet_noise);

    std::vector<float> search(const Board &board, int simulate_num, int T);

    void change_root(int action);

public:
    MctsStrategy(conf::MctsConf mcts_conf, Player player, shared_ptr<Expert> expert, bool sample=true);

    void post_process(const Board &board) override;

    std::tuple<int, int> step(const Board &board, StepRecord &record) override;
};


#endif //ALPHAZERO_RENJU_MCTS_STRATEGY_H
