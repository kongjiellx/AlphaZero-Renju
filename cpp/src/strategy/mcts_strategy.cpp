//
// Created by 刘也宽 on 2020/11/18.
//

#include "mcts_strategy.h"
#include "cpp/src/utils/dirichlet.h"
#include <numeric>
#include <random>
#include <iostream>

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
: root(new Node(nullptr, 0, Player::O)), mcts_conf(mcts_conf), current_step(0) {}

std::tuple<int, int> MctsStrategy::step(const Board& board) {
    float t;
    if (current_step < mcts_conf.explore_steps()) {
        t = 1;
    } else {
        t = 0.5;
    }
    auto ps = search(board, 100, t, true);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> distribution(ps.begin(), ps.end());
    auto pos = distribution(gen);
    std::cout << pos << std::endl;
    return std::tuple<int, int>(pos / board.get_size(), pos % board.get_size());
}

void MctsStrategy::change_root(int action) {
    if (root->getChildren().find(action) != root->getChildren().end()) {
        root = root->getChildren()[action];
    } else {
        root = new Node(nullptr, 0, root->getPlayer() == Player::X ? Player::O : Player::X);
    }
}

std::vector<float> MctsStrategy::search(const Board &board, int simulate_num, int T, bool add_dirichlet_noise) {
    for (int i = 0; i < simulate_num; i++) {
        std::cout << i << std::endl;
        Board copy_board = Board(board);
        Node *node = root;
        std::tuple<bool, Player> status;
        while (!node->is_leaf()) {
            auto node_action = node->select();
            node = std::get<0>(node_action);
            int action = std::get<1>(node_action);
            status = copy_board.step(Stone(
                    action / copy_board.get_size(),
                    action % copy_board.get_size(),
                    copy_board.current_player));
        }
        float v;
        if (std::get<0>(status)) {
            v = std::get<1>(status);
        } else {
            std::vector<float> ps(
                    copy_board.get_size() * copy_board.get_size(),
                    1.0f / copy_board.get_size() * copy_board.get_size());
            v = 0.5;
            if (add_dirichlet_noise && i == 0) {
                dirichlet_noise(ps);
            }
            for (auto& idx: copy_board.get_illegal_idx()) {
                ps[idx] = 0;
            }
            auto sum = std::accumulate(ps.begin(), ps.end(), 0);
            for (auto& p: ps) {
                p /= sum;
            }
            node->expand(ps, copy_board.current_player);
        }
        node->backup(v);
    }
    std::vector<float> ret(board.get_size() * board.get_size(), 0);
    for (auto it: root->getChildren()) {
        ret[it.first] = it.second->getN();
    }
    auto denominators(ret);
    for (auto &it: denominators) {
        it = std::pow(it, 1.0 / T);
    }
    auto sum = std::accumulate(denominators.begin(), denominators.end(), 0);
    for (auto &it: ret) {
        it = std::pow(it, 1.0 / T) / sum;
    }
    return ret;
}

void MctsStrategy::dirichlet_noise(std::vector<float>& ps) {
    int dim = ps.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    dirichlet_distribution<std::mt19937> d(std::vector<double>(dim, 1));
    auto dirichlet_noise = d(gen);
    for (int i = 0; i < dim; i++) {
        ps[i] = (1 - mcts_conf.dirichlet_esp()) * ps[i] + mcts_conf.dirichlet_esp() * dirichlet_noise[i];
    }
}
