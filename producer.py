from board import Board, Player, Stone
from mcts import MCTS
import conf
import numpy as np


class Producer(object):
    def __init__(self):
        self.data = []

    def playn(self, n, net):
        ds, ps, vs = [], [], []
        for i in range(n):
            x, y1, y2 = self.play_a_agme(net, True)
            ds.extend(x)
            ps.extend(y1)
            vs.extend(y2)
        return ds, ps, vs

    def play_a_agme(self, net, paint=False):
        board = Board()
        mcts = MCTS()
        o_data, x_data = [], []
        o_ps, x_ps = [], []
        while True:
            feature = board.get_feature()
            p = mcts.simulate(net, 200)
            if board.turn == Player.O:
                o_data.append(feature)
                o_ps.append(p)
            for i in board.illegal_idx:
                p[i] = 0
            idx = np.random.choice(conf.board_size ** 2, p=p)
            pos = (idx // conf.board_size, idx % conf.board_size)
            mcts.change_root(idx)
            stone = Stone(pos, board.turn)
            is_done, winner = board.step(stone)
            if paint:
                print(board)
            if is_done:
                if paint:
                    if winner == Player.O:
                        print("O WIN!")
                    elif winner == Player.X:
                        print("X WIN!")
                    else:
                        print("NO WINNER!")
                if winner == Player.O:
                    o_v = [1] * len(o_data)
                    x_v = [-1] * len(x_data)
                    return o_data + x_data, o_ps + x_ps, o_v + x_v
                elif winner == Player.X:
                    o_v = [-1] * len(o_data)
                    x_v = [1] * len(x_data)
                    return o_data + x_data, o_ps + x_ps, o_v + x_v
                else:
                    return [], [], []
