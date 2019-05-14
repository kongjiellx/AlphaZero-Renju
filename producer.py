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
        steps = 0
        board = Board()
        mcts = MCTS()
        o_data, x_data = [], []
        o_ps, x_ps = [], []
        while True:
            feature = board.get_feature()
            p = mcts.simulate(
                net=net,
                simulate_num=300,
                T=1 if steps < conf.explore_steps else 0.5,
                add_dirichlet_noise=True if steps < conf.explore_steps else False,
            )

            # debug
            ps, v = net.predict(np.array([feature]))
            print("==========V==========")
            print(v)
            print("==========ps==========")
            info = ''
            for i in range(1, len(ps) + 1):
                info += "%.2f" % ps[i - 1] + ' '
                if i % conf.board_size == 0:
                    info += '\n'
            print(info)
            print("==========mcts==========")
            info = ''
            for i in range(1, len(p) + 1):
                info += "%.2f" % p[i - 1] + ' '
                if i % conf.board_size == 0:
                    info += '\n'
            print(info)
            # debug

            if board.turn == Player.O:
                o_data.append(feature)
                o_ps.append(p)
            else:
                x_data.append(feature)
                x_ps.append(p)
            idx = np.random.choice(conf.board_size ** 2, p=p)
            pos = (idx // conf.board_size, idx % conf.board_size)
            mcts.change_root(idx)
            stone = Stone(pos, board.turn)
            is_done, winner = board.step(stone)
            steps += 1
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
