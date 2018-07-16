from setting import *
import copy
import numpy as np
import random
from board import Stone
from mxnet import nd


class Node(object):
    def __init__(self, board, net, inEdge):
        self.board = board
        self.net = net
        self.edges = {}
        self.nodes = {}
        self.inEdge = inEdge

    def evaluate(self):
        ps, v = self.net(nd.array(np.expand_dims(self.board.get_feature(), axis=0)))
        # print(ps, v)
        # ps = [1.0 / board_size ** 2] * board_size ** 2
        # v = 0
        return ps.asnumpy()[0], v.asnumpy()[0][0]

    def select(self):
        nb = sum([edge.N for action, edge in self.edges.items()])
        maxQU = -9999
        for idx, edge in self.edges.items():
            U = CPUCT * edge.P * np.sqrt(nb) / (1 + edge.N)
            if edge.Q + U > maxQU:
                maxQU = U + edge.Q
                select_action = idx
        return self.nodes[select_action]

    def expand(self):
        ps, v = self.evaluate()
        for idx, p in enumerate(ps):
            if idx in self.board.illegal_idx:
                continue
            self.edges[idx] = Edge(self, p)
            cp_board = copy.deepcopy(self.board)
            pos = (idx // board_size, idx % board_size)
            self.nodes[idx] = Node(cp_board.step(Stone(pos, cp_board.turn)), self.net, self.edges[idx])
        return v

    def backup(self, v, turn):
        if self.inEdge:  # not root
            self.inEdge.N += 1
            if self.board.turn == turn:
                self.inEdge.W += v
            else:
                self.inEdge.W -= v
            self.inEdge.Q = self.inEdge.W / self.inEdge.N
            self.inEdge.inNode.backup(v, turn)

    def isLeaf(self):
        if len(self.edges) > 0:
            return False
        else:
            return True


class Edge(object):
    def __init__(self, inNode, p):
        self.inNode = inNode
        self.N = 0
        self.W = 0
        self.Q = 0
        self.P = p


class MTCS(object):
    def __init__(self, net, board):
        self.root = Node(board, net, None)
        self.T = 0.5

    def move_to_leaf(self):
        node = self.root
        while not node.isLeaf():
            node = node.select()
        return node

    def simulate(self, simulate_num):
        for i in range(simulate_num):
            node = self.move_to_leaf()
            v = node.expand()
            node.backup(v, node.board.turn)
        ret = [0] * board_size ** 2
        for idx, n in self.root.nodes.items():
            ret[idx] = n.inEdge.N
        return np.array(ret) ** (1.0 / self.T) / sum(np.array(ret) ** (1.0 / self.T))

    def change_root(self, action):
        self.root = self.root.nodes[action]
