from setting import *
from game import Stone
import numpy as np
import copy


class MTCS(object):
    def __init__(self, net, board):
        self.net = net
        self.board = board

    def get_label(self):
        for i in range(self.board.size):
            for j in range(self.board.size):
                stone = Stone((i, j), self.board.turn)
                if self.board.is_leagal(stone):
                    for n in range(rollout_num):
                        fake_board = copy.deepcopy(self.board)
                        fake_board.add_stone(stone)
                        while True:
                            out = self.net(fake_board.get_feature())
                            while True:
                                pred = np.random.choice(num_outputs, 1, p=out)
                                pos = (pred / board_size, pred % board_size)
                                st = Stone(pos, fake_board.do_turn())
                                if fake_board.is_legal(st):
                                    fake_board.add_stone(st)
                                    break
        label = label.as_in_context(ctx)
        with autograd.record():
            output = net(data.as_in_context(ctx))
            loss = softmax_cross_entropy(output, label)
        loss.backward()
        pass
