import random
from setting import board_size
from board import Stone
from mcts import MTCS
from model import ResNet
import numpy as np


class RandomAgent(object):
    def step(self, board):
        pos = (random.randint(0, board_size - 1), random.randint(0, board_size - 1))
        return Stone(pos, board.turn)


class MCTSAgent(object):
    def __init__(self, net, board):
        self.mtcs = MTCS(net, board)

    def step(self, board):
        ps = self.mtcs.simulate(200)
        illegal_idx = board.illegal_idx
        for i in illegal_idx:
            ps[i] = 0
        idx = np.random.choice(board_size ** 2, p=ps)
        pos = (idx // board_size, idx % board_size)
        self.mtcs.change_root(idx)
        return ps, Stone(pos, board.turn)