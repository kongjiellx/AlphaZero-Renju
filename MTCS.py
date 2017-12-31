from setting import *
from game import Stone, Color
import numpy as np
import copy
from mxnet import nd


def softmax(x):
    return np.exp(x) / np.sum(np.exp(x))


class MTCS(object):
    def __init__(self, net, board, ctx):
        self.net = net
        self.board = board
        self.ctx = ctx

    def get_label(self):
        label = np.zeros(shape=(self.board.size, self.board.size))
        for i in range(self.board.size):
            for j in range(self.board.size):
                f_board = copy.deepcopy(self.board)
                stone = Stone((i, j), self.board.turn)
                true_status = f_board.step(stone)
                if true_status == 0:
                    for n in range(rollout_num):
                        fake_board = copy.deepcopy(f_board)
                        while True:
                            out = self.net(nd.array(fake_board.get_feature(), ctx=self.ctx)).asnumpy().reshape(
                                num_outputs)
                            pred = np.random.choice(num_outputs, 1, p=softmax(out))
                            # pred = np.argmax(out)
                            pos = (int(pred / board_size), int(pred % board_size))
                            st = Stone(pos, fake_board.turn)
                            status = fake_board.step(st)
                            if status == 0:
                                # print('go on')
                                continue
                            elif status == 2:
                                # print("no one win")
                                break
                            elif status == stone.color:
                                # print("%s win" % ("black" if stone.color == Color.black else "white"))
                                label[i][j] += 1
                                break
                            elif status == -stone.color:
                                label[i][j] -= 1
                                break
                elif true_status == 2:
                    continue
                elif true_status == stone.color:
                    label[i][j] += rollout_num
                elif true_status == -stone.color:
                    label[i][j] -= rollout_num
        # print("%s: %s" % ("black" if self.board.turn == Color.black else "white", label))
        label = softmax(label.reshape(1, num_outputs))
        return label
