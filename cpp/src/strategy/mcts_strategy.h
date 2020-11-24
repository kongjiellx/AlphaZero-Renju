//
// Created by 刘也宽 on 2020/11/18.
//

#ifndef ALPHAZERO_RENJU_MCTS_STRATEGY_H
#define ALPHAZERO_RENJU_MCTS_STRATEGY_H

#include "strategy.h"
#include "conf/conf_cc_proto_pb/conf/conf.pb.h"
#include <tuple>
#include <unordered_map>

class Node {
private:
    Node* parent;
    int N;
    float W;
    float P;
    Player player;
    std::unordered_map<int, Node*> children;
public:
    Player getPlayer() const;
    void setParent(Node *parent);
    std::unordered_map<int, Node *> &getChildren();
    Node(Node* parent, float p, Player player);
    ~Node();
    float Q();
    int getN();
    float getP();
    std::tuple<Node*, int> select();
    void expand(std::vector<float> ps, Player player);
    void backup(float v);
    bool is_leaf();
};

class MctsStrategy: public Strategy {
private:
    Node* root;
    Node* current_root;
    int current_step;
    conf::MctsConf mcts_conf;
    void dirichlet_noise(std::vector<float>& ps);
    std::vector<float> search(const Board& board, int simulate_num, int T, bool add_dirichlet_noise);
    void change_root(int action);
public:
    MctsStrategy(conf::MctsConf mcts_conf);
    ~MctsStrategy();

    void post_process(const Board &board) override;

    std::tuple<int, int> step(const Board& board) override;
};


#endif //ALPHAZERO_RENJU_MCTS_STRATEGY_H
