import copy
import numpy as np
import math
import random
from board import Stone, Board
import conf


class Node(object):
    def __init__(self, board, parent, p, is_done, winner):
        self.parent = parent
        self.N = 0
        self.W = 0
        self.Q = 0
        self.P = p

        self.board = board
        self.children = {}
        self.is_done = is_done
        self.winner = winner
        self.v = None

    def select(self):
        nb = sum([child.N for action, child in self.children.items()])
        max_qu = -1
        for idx, child in self.children.items():
            U = conf.CPUCT * child.P * math.sqrt(nb) / (1 + child.N)
            if child.Q + U > max_qu:
                max_qu = U + child.Q
                select_action = idx
        return self.children[select_action]

    def expand(self, ps):
        for idx, p in enumerate(ps):
            if idx in self.board.illegal_idx:
                continue
            cp_board = copy.deepcopy(self.board)
            pos = (idx // conf.board_size, idx % conf.board_size)
            is_done, winner = cp_board.step(Stone(pos, cp_board.turn))
            self.children[idx] = Node(cp_board, self, p, is_done, winner)

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
        state = self.board.__str__()
        info = "N: %s, W: %s, Q: %s, P: %s\n" % (self.N, self.W, self.Q, self.P)
        return state + info


class MCTS(object):
    def __init__(self):
        self.root = Node(board=Board(), parent=None, p=None, is_done=False, winner=None)

    def move_to_leaf(self):
        node = self.root
        while not node.is_leaf():
            node = node.select()
        return node

    def dirichlet_noise(self, ps):
        """ Add Dirichlet noise in the root node """

        dim = (ps.shape[0],)
        new_ps = (1 - conf.dirichlet_esp) * ps + \
                        conf.dirichlet_esp * np.random.dirichlet(np.full(dim, conf.dirichlet_alpha))
        return new_ps

    def simulate(self, net, simulate_num, T):
        for i in range(simulate_num):
            node = self.move_to_leaf()
            if node.v is not None:
                v = node.v
            else:
                feature = np.expand_dims(node.board.get_feature(), axis=0)
                ps, v = net.predict(feature)
                node.v = v
                if i == 0:
                    ps = self.dirichlet_noise(ps)
                node.expand(ps)
            node.backup(v)
        ret = [0] * conf.board_size ** 2
        for idx, child in self.root.children.items():
            ret[idx] = child.N
        # print search results
        print("==========N==========")
        info = ''
        for i in range(1, len(ret) + 1):
            info += str(ret[i - 1]) + ' '
            if i % conf.board_size == 0:
                info += '\n'
        print(info)
        return np.array(ret) ** (1.0 / T) / sum(np.array(ret) ** (1.0 / T))

    def change_root(self, action):
        self.root = self.root.children[action]

