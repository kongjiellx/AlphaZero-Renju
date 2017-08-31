from board import Board
from model import Model
import numpy as np


def one_hot(x):
    ret = []
    for n in x:
        t = np.zeros(25)
        t[n] = 1
        ret.append(t)
    return np.array(ret)


class Game(object):
    def __init__(self):

        self.model = Model()
        self.model.load()

    def play(self):
        ps = 0
        for i in range(1000000):
            if i % 10000 == 0:
                self.model.save()
                print(ps)
                ps = 0

            self.board = Board()
            # print('*******************')
            observation = self.board.board
            done = False
            sum_reward = 0
            predicts = []
            points = []
            X = []
            while(not done):
                X.append(observation.flatten())
                predict = self.model.forward(observation.flatten())
                point = np.argmax(predict)
                predicts.append(predict)
                points.append(point)
                done, reward, observation = self.board.step((point // 5, point % 5))
                sum_reward += reward
                # print(observation)
                # print('______________')
            grad = self.model.backward(sum_reward, X, one_hot(points), predicts)
            self.model.update(grad, 0.0001)
            ps += sum_reward
        self.model.save()

if __name__ == '__main__':
    g = Game()
    g.play()
