//
// Created by 刘也宽 on 2020/11/18.
//

#include "mcts_strategy.h"
#include "cpp/src/utils/dirichlet.h"
#include "glog/logging.h"
#include <numeric>
#include <random>
#include <iostream>
#include <sstream>

Node::Node(Node *parent, float p, Player player)
:parent(parent), N(0), W(0), P(p), player(player) {
}

Node::~Node() {
    for(auto it: children) {
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
    int num = 0;
    std::stringstream dstr;
    for (int i = 0; i < ps.size(); i++) {
        if (ps[i] > 0) {
            num ++;
            dstr << i << ",";
            children[i] = new Node(this, ps[i], player);
        }
    }
    DLOG(INFO) << "NUM: " << num;
    DLOG(INFO) << "expend: " << dstr.str();
}

void Node::backup(float v) {
    N += 1;
    W += v;
    if (parent != nullptr) {
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
    float max_qu = -1;
    int select_action = -1;
    for (auto it: children) {
        float U = 5 * it.second->getP() * std::sqrt(nb) / (1 + it.second->getN());
        float child_q = it.second->Q();
        if (child_q + U > max_qu) {
            max_qu = U + child_q;
            select_action = it.first;
        }
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

MctsStrategy::MctsStrategy(conf::MctsConf mcts_conf, Player player)
: Strategy(player), root(new Node(nullptr, 0, player)), current_root(root), mcts_conf(mcts_conf), current_step(0) {}

std::tuple<int, int> MctsStrategy::step(const Board& board, StepRecord& record) {
    float t;
    if (current_step < mcts_conf.explore_steps()) {
        t = 1;
    } else {
        t = 0.5;
    }
    auto ps = search(board, 100, t, true);
    record.distribution = ps;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> distribution(ps.begin(), ps.end());
    auto pos = distribution(gen);
    change_root(pos);
    return std::tuple<int, int>(pos / board.get_size(), pos % board.get_size());
}

void MctsStrategy::change_root(int action) {
    if (current_root -> getChildren().find(action) != root->getChildren().end()) {
        current_root = current_root->getChildren()[action];
        current_root->setParent(nullptr);
    } else {
        current_root = new Node(nullptr, 0, root->getPlayer() == Player::O ? Player::X : Player::O);
        delete root;
        root = current_root;
    }

}

std::vector<float> MctsStrategy::search(const Board &board, int simulate_num, int T, bool add_dirichlet_noise) {
    DLOG(INFO) << "Search start!";
    for (int i = 0; i < simulate_num; i++) {
        DLOG(INFO) << "simulate: " << i;
        Board copy_board(board);
        Node *node = current_root;
        std::tuple<bool, Player> status;
        while (!node->is_leaf()) {
            auto node_action = node->select();
            node = std::get<0>(node_action);
            int action = std::get<1>(node_action);
            DLOG(INFO) << "select action: " << action;
            status = copy_board.step(Stone(
                    action / copy_board.get_size(),
                    action % copy_board.get_size(),
                    copy_board.current_player));
        }
        float v;
        if (std::get<0>(status)) {
            v = std::get<1>(status);
            DLOG(INFO) << "Get done leaf, v: " << v;
        } else {
            std::vector<float> ps(
                    copy_board.get_size() * copy_board.get_size(),
                    1.0f / (copy_board.get_size() * copy_board.get_size()));
            v = 0.5;
            DLOG(INFO) << "Get leaf, v: " << v;

            if (add_dirichlet_noise && i == 0) {
                dirichlet_noise(ps);
            }

            std::stringstream ill_str;
            for (auto& idx: copy_board.get_illegal_idx()) {
                ps[idx] = 0;
                ill_str << idx << ",";
            }
            auto sum = std::accumulate(ps.begin(), ps.end(), 0.0);
            for (auto& p: ps) {
                p /= sum;
            }
            DLOG(INFO) << "illegal_idx: " << ill_str.str();
            node->expand(ps, copy_board.current_player);
        }
        node->backup(v);
    }
    std::vector<float> ret(board.get_size() * board.get_size(), 0);
    for (auto it: current_root->getChildren()) {
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

MctsStrategy::~MctsStrategy() {
    delete root;
}

void MctsStrategy::post_process(const Board &board) {
    auto pos = board.get_last_pos();
    change_root(std::get<0>(pos) * board.get_size() + std::get<1>(pos));
}
