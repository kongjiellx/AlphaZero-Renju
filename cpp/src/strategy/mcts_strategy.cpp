#include "mcts_strategy.h"
#include "cpp/src/utils/dirichlet.h"
#include "cpp/src/model_manager.h"
#include <numeric>
#include <random>
#include <iostream>
#include <cmath>

Node::Node(weak_ptr<Node> parent, float p, Player player)
        : parent(parent), N(0), W(0), P(p), player(player) {
}

float Node::Q() {
    if (N > 0) {
        return W / N * player;
    } else {
        return 0;
    }
}

void Node::expand(vector<float> ps, Player player) {
    for (int i = 0; i < ps.size(); i++) {
        if (ps[i] > 0) {
            children[i] = make_shared<Node>(shared_from_this(), ps[i], player);
        }
    }
}

void Node::backup(float v) {
    N += 1;
    W += v;
    auto p = parent.lock();
    if (p) {
        p->backup(v);
    }
}

bool Node::is_leaf() {
    return children.size() == 0;
}

tuple<shared_ptr<Node>, int> Node::select() {
    int nb = 0;
    for (auto &it: children) {
        nb += it.second->getN();
    }
    float max_qu = -1;
    int select_action = -1;
    for (auto &it: children) {
        float U = 5 * it.second->getP() * std::sqrt(nb) / (1 + it.second->getN());
        float child_q = it.second->Q();
        if (child_q + U > max_qu) {
            max_qu = U + child_q;
            select_action = it.first;
        }
    }
    return std::make_tuple(children[select_action], select_action);
}

int Node::getN() {
    return N;
}

float Node::getP() {
    return P;
}

std::unordered_map<int, shared_ptr<Node>> &Node::getChildren() {
    return children;
}

void Node::setParent(weak_ptr<Node> parent) {
    this->parent = parent;
}

Player Node::getPlayer() const {
    return player;
}

MctsStrategy::MctsStrategy(conf::MctsConf mcts_conf, Player player, MODEL_TYPE model_type, bool with_lock, bool sample)
        : Strategy(player), root(new Node(weak_ptr<Node>(), 0, player)), current_root(root), mcts_conf(mcts_conf),
          current_step(0), model_type(model_type), with_lock(with_lock), sample(sample) {}


std::tuple<int, int> MctsStrategy::step(const Board &board, StepRecord &record) {
    float t;
    if (current_step < mcts_conf.explore_steps()) {
        t = 1;
    } else {
        t = 0.5;
    }
    const auto &ps = search(board, mcts_conf.simulate_num(), t);
    record.distribution = ps;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> distribution(ps.begin(), ps.end());
    int pos;
    if (sample) {
        pos = distribution(gen);
    } else {
        pos = std::max_element(ps.begin(), ps.end()) - ps.begin();
    }
    return std::make_tuple(pos / board.get_size(), pos % board.get_size());
}

void MctsStrategy::change_root(int action) {
    if (current_root->getChildren().find(action) != root->getChildren().end()) {
        current_root = current_root->getChildren()[action];
        current_root->setParent(weak_ptr<Node>());
    } else {
        current_root = make_shared<Node>(weak_ptr<Node>(), 0, root->getPlayer() == Player::O ? Player::X : Player::O);
        root = current_root;
    }
}

void MctsStrategy::simulate(const Board &board, bool add_dirichlet_noise) {
    Board copy_board(board);
    shared_ptr<Node> node = current_root;
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
        auto features = board_status_to_feature(copy_board.get_current_status(),
                                                            copy_board.current_player);
        auto pv = ModelManager::instance().predict(*features, model_type, with_lock);
        std::vector<float> ps = std::get<0>(*pv);
        v = std::get<1>(*pv);

        if (add_dirichlet_noise) {
            dirichlet_noise(ps);
        }
        for (auto &idx: copy_board.get_illegal_idx()) {
            ps[idx] = 0;
        }
        auto sum = std::accumulate(ps.begin(), ps.end(), 0.0);
        for (auto &p: ps) {
            p /= sum;
        }
        node->expand(ps, copy_board.current_player);
    }
    node->backup(v);    
}

std::vector<float> MctsStrategy::search(const Board &board, int simulate_num, int T) {
    for (int i = 0; i < simulate_num; i++) {
        simulate(board, i == 0 && current_root == root);
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

void MctsStrategy::dirichlet_noise(std::vector<float> &ps) {
    int dim = ps.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    dirichlet_distribution<std::mt19937> d(std::vector<double>(dim, mcts_conf.dirichlet_alpha()));
    auto dirichlet_noise = d(gen);
    for (int i = 0; i < dim; i++) {
        ps[i] = (1 - mcts_conf.dirichlet_esp()) * ps[i] + mcts_conf.dirichlet_esp() * dirichlet_noise[i];
    }
}

void MctsStrategy::post_process(const Board &board) {
    auto pos = board.get_last_pos();
    change_root(std::get<0>(pos) * board.get_size() + std::get<1>(pos));
}

