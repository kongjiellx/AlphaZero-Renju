//
// Created by 刘也宽 on 2020/11/18.
//

#include "mcts_strategy.h"
#include "cpp/src/utils/dirichlet.h"

Node::Node(Node *parent, float p, Player player)
:parent(parent), N(0), W(0), P(p), player(player) {
}

Node::~Node() {
    for(auto it: children) {
        it.second -> ~Node();
        delete it.second;
    }
    children.clear();
}

float Node::Q() {
    if (N > 0) {
        return W / N * player;
    } else {
        return 0;
    }
}

void Node::expand(std::vector<float> ps, Player player) {
    for (int i = 0; i < ps.size(); i++) {
        children[i] = new Node(this, ps[i], player);
    }
}

void Node::backup(float v) {
    N += 1;
    W += v;
    if (parent != NULL) {
        parent -> backup(v);
    }
}

bool Node::is_leaf() {
    return children.size() == 0;
}

std::tuple<Node*, int> Node::select() {
    int nb = 0;
    for (auto it: children) {
        nb += it.second->getN();
    }
    int i = 0;
    float max_qu = -1;
    int select_action = -1;
    for (auto it: children) {
        float U = 5 * it.second->getP() * std::sqrt(nb) / (1 + it.second->getN());
        float child_q = it.second->Q();
        if (child_q + U > max_qu) {
            max_qu = U + child_q;
            select_action = i;
        }
        i++;
    }
    return std::tuple<Node*, int>(children[select_action], select_action);
}

int Node::getN() {
    return N;
}

float Node::getP() {
    return P;
}

std::unordered_map<int, Node *> &Node::getChildren() {
    return children;
}

void Node::setParent(Node *parent) {
    Node::parent = parent;
}

Player Node::getPlayer() const {
    return player;
}

MctsStrategy::MctsStrategy(conf::MctsConf mcts_conf)
: root(new Node(nullptr, 0, Player::O)), mcts_conf(mcts_conf) {}

std::tuple<int, int> MctsStrategy::step(const Board& board) {
    return std::tuple<int, int>(0, 0);
}

void MctsStrategy::change_root(int action) {
    if (root->getChildren().find(action) != root->getChildren().end()) {
        root = root->getChildren()[action];
    } else {
        root = new Node(nullptr, 0, root->getPlayer() == Player::X ? Player::O : Player::X);
    }
}

std::vector<float> MctsStrategy::search(Board &board, int simulate_num) {
    return std::vector<float>();
}

std::vector<float> MctsStrategy::dirichlet_noise(std::vector<float> ps) {
    int dim = ps.size();
    std::vector<float> new_ps(ps);

    std::random_device rd;
    std::mt19937 gen(rd());
    dirichlet_distribution<std::mt19937> d(std::vector<double>(dim, 1));
    auto dirichlet_noise = d(gen);
    for (int i = 0; i < dim; i++) {
        new_ps[i] = (1 - mcts_conf.dirichlet_esp()) * new_ps[i] + mcts_conf.dirichlet_esp() * dirichlet_noise[i];
    }
    return new_ps;
}
