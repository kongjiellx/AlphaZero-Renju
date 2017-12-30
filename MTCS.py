from setting import *
from game import Stone, Color
import numpy as np
import copy
from mxnet import nd


class MTCS(object):
    def __init__(self, net, board, ctx):
        self.net = net
        self.board = board
        self.ctx = ctx

    def get_label(self):
        label = np.zeros(shape=(self.board.size, self.board.size))
        for i in range(self.board.size):
            for j in range(self.board.size):
                stone = Stone((i, j), self.board.turn)
                if self.board.is_leagal(stone):
                    for n in range(rollout_num):
                        fake_board = copy.deepcopy(self.board)
                        fake_board.add_stone(stone)
                        fake_board.do_turn()
                        while True:
                            out = self.net(nd.array(fake_board.get_feature(), ctx=self.ctx)).asnumpy()
                            while True:
                                pred = np.random.choice(num_outputs, 1, p=out.reshape(num_outputs))
                                pos = (int(pred / board_size), int(pred % board_size))
                                st = Stone(pos, fake_board.turn)
                                if fake_board.is_leagal(st):
                                    fake_board.add_stone(st)
                                    fake_board.do_turn()
                                    # fake_board.paint()
                                    break
                            winer = fake_board.is_over(st)
                            if winer == Color.black:
                                label[i][j] += 1
                                print("black win")
                                break
                            elif winer == Color.white:
                                print("white win")
                                break
                            elif winer == 2:
                                print("no one win")
                                break
        print(label)
        # label = label.as_in_context(ctx)
        # with autograd.record():
        #     output = net(data.as_in_context(ctx))
        #     loss = softmax_cross_entropy(output, label)
        # loss.backward()
        # pass
