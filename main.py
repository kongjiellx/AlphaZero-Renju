from board import Board, Player, Stone
from agent import RandomAgent, MCTSAgent
from model import ResNet
from setting import board_size, batch_size
from mxnet import autograd
from mxnet import gluon
from mxnet import init
from mxnet import nd
import mxnet as mx
import numpy as np
import datetime


class Game(object):
    def train_batch(self, net, x, ps, v):
        dataset_train = gluon.data.ArrayDataset(x, list(zip(ps, v)))
        train_data = gluon.data.DataLoader(dataset_train, batch_size=batch_size, shuffle=True)
        trainer = gluon.Trainer(
            net.collect_params(), 'sgd', {'learning_rate': 0.01, 'momentum': 0.9, 'wd': 0.00001})
        softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss(sparse_label=False)
        l2loss = gluon.loss.L2Loss()
        print(x, ps, v)
        for i in range(1):
            for data, label in train_data:
                with autograd.record():
                    output = net(data)
                    loss = softmax_cross_entropy(output[0], label[0]) + l2loss(output[1], label[1])
                loss.backward()
                trainer.step(batch_size)
                print("loss: %s" % nd.mean(loss).asscalar())


    def run(self):
        net = ResNet(board_size ** 2)
        net.collect_params().initialize(init=init.Xavier(), ctx=mx.cpu())
        for i in range(1000):
            board = Board()
            agent = MCTSAgent(net, board)
            o_data, x_data = [], []
            o_ps, x_ps = [], []
            o_v, x_v = [], []
            winner = None
            while len(board.illegal_idx) < 36:
                o_data.append(board.get_feature())
                ps, o_stone = agent.step(board)
                o_ps.append(ps)
                board.step(o_stone)
                board.paint()
                if board.get_winer(o_stone) == Player.O:
                    print("O WIN!")
                    winner = Player.O
                    break
                x_data.append(board.get_feature())
                ps, x_stone = agent.step(board)
                x_ps.append(ps)
                board.step(x_stone)
                board.paint()
                if board.get_winer(x_stone) == Player.X:
                    print("X WIN!")
                    winner = Player.X
                    break
            if winner == Player.O:
                o_v = [1] * len(o_data)
                x_v = [-1] * len(x_data)
            else:
                o_v = [-1] * len(o_data)
                x_v = [1] * len(x_data)
            self.train_batch(net,
                             np.array(o_data + x_data, dtype=np.float32),
                             np.array(o_ps + x_ps, dtype=np.float32),
                             np.array(o_v + x_v, dtype=np.float32))


if __name__ == "__main__":
    game = Game()
    game.run()