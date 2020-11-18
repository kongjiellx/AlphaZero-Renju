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
public:
    void setParent(Node *parent);

private:
    int N;
    float W;
    float P;
    Player player;
public:
    Player getPlayer() const;

private:
    std::unordered_map<int, Node*> children;
public:
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
    conf::MctsConf mcts_conf;
    std::vector<float> dirichlet_noise(std::vector<float> ps);
    std::vector<float> search(Board& board, int simulate_num, int T, bool add_dirichlet_noise);
    void change_root(int action);
public:
    MctsStrategy(conf::MctsConf mcts_conf);

    std::tuple<int, int> step(const Board& board) override;
};


#endif //ALPHAZERO_RENJU_MCTS_STRATEGY_H
