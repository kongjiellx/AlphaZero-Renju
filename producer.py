from board import Board, Player, Stone
from mcts import MCTS
import conf
import numpy as np
from collections import deque
import random


class Producer(object):
    def __init__(self):
        self.data = deque(maxlen=conf.maxlen)

    def data_augmentation(self, data):
        new_data = []
        for x, y1, y2 in data:
            for i in [1, 2, 3, 4]:
                new_data.append((
                    np.rot90(x, i),
                    np.rot90(y1.reshape((conf.board_size, conf.board_size)), i).reshape(conf.num_outputs),
                    y2
                ))
                new_data.append((
                    x[::-1, :, :],
                    y1.reshape((conf.board_size, conf.board_size))[::-1, :].reshape(conf.num_outputs),
                    y2
                ))
        return new_data

    def get_sample_data(self, n):
        mini_batch = random.sample(self.data, n)
        return [item[0] for item in mini_batch], \
            [item[1] for item in mini_batch], \
            [item[2] for item in mini_batch]

    def playn(self, n, net):
        for i in range(n):
            self.play_a_agme(net, True)

    def play_a_agme(self, net, paint=False):
        steps = 0
        board = Board()
        mcts = MCTS()
        o_data, x_data = [], []
        o_ps, x_ps = [], []
        while True:
            feature = board.get_feature()
            p = mcts.search(
                board=board,
                net=net,
                simulate_num=conf.simulate_num,
                T=1 if steps < conf.explore_steps else 0.5,
                add_dirichlet_noise=True,
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
                elif winner == Player.X:
                    o_v = [-1] * len(o_data)
                    x_v = [1] * len(x_data)
                else:
                    o_v = [0] * len(o_data)
                    x_v = [0] * len(x_data)
                self.data.extend(
                    self.data_augmentation(
                        zip(o_data + x_data, o_ps + x_ps, o_v + x_v)
                    )
                )
                break
