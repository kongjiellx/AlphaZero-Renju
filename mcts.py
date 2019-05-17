import copy
import numpy as np
import math
import random
from board import Stone, Board, Player
import conf


class Node(object):
    def __init__(self, parent, p):
        self.parent = parent
        self.N = 0
        self.W = 0
        self.Q = 0
        self.P = p

        self.children = {}

    def select(self):
        nb = sum([child.N for action, child in self.children.items()])
        max_qu = -1
        for idx, child in self.children.items():
            U = conf.CPUCT * child.P  * math.sqrt(nb) / (1 + child.N)
            if child.Q + U > max_qu:
                max_qu = U + child.Q
                select_action = idx
        return self.children[select_action], select_action

    def expand(self, ps):
        for idx, p in enumerate(ps):
            if p > 0:
                self.children[idx] = Node(self, p)

    def backup(self, v):
        self.N += 1
        self.W += v
        self.Q = self.W / self.N
        if self.parent is not None:  # not root
            self.parent.backup(v)

    def is_leaf(self):
        if len(self.children) > 0:
            return False
        else:
            return True

    def __str__(self):
        info = "N: %s, W: %s, Q: %s, P: %s" % (self.N, self.W, self.Q, self.P)
        return info


class MCTS(object):
    def __init__(self):
        self.root = Node(parent=None, p=None)

    def dirichlet_noise(self, ps):
        """ Add Dirichlet noise in the root node """

        dim = (ps.shape[0],)
        new_ps = (1 - conf.dirichlet_esp) * ps + \
            conf.dirichlet_esp * np.random.dirichlet(np.full(dim, conf.dirichlet_alpha))
        return new_ps

    def search(self, board, net, simulate_num, T, add_dirichlet_noise):
        for i in range(simulate_num):
            cp_board = copy.deepcopy(board)

            # move to leaf
            node = self.root
            is_done, winner = None, None
            while not node.is_leaf():
                node, action = node.select()
                pos = (action // conf.board_size, action% conf.board_size)
                is_done, winner = cp_board.step(Stone(pos, cp_board.turn))
            if is_done and hasattr(node, "v"):
                v = node.v
            else:
                ps, v = net.predict(np.array([cp_board.get_feature()]))
                node.v = v
                if add_dirichlet_noise and i == 0:
                    ps = self.dirichlet_noise(ps)
                for idx in cp_board.illegal_idx:
                    ps[idx] = 0
                ps /= sum(ps)
                node.expand(ps)
            node.backup(v)
        ret = [0] * conf.board_size ** 2
        for idx, child in self.root.children.items():
            ret[idx] = child.N
        return np.array(ret) ** (1.0 / T) / sum(np.array(ret) ** (1.0 / T))

    def change_root(self, action):
        self.root = self.root.children[action]
        self.root.parent = None

