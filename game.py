from board import Board
from model import Model
import numpy as np


def one_hot(x):
    ret = np.zeros(25)
    ret[x] = 1
    return ret


class Game(object):
    def __init__(self):
        self.board = Board()
        self.model = Model()

    def play(self):
        observation = self.board.board
        done = False
        for i in range(10):
            sum_reward = 0
            while(not done):
                predict = self.model.forward(observation.flatten())
                point = np.argmax(predict)
                done, reward, observation = self.board.step((point // 5, point % 5))
                sum_reward += reward
                print(observation)
            grad = self.model.backward(sum_reward, one_hot(point), predict)
            self.model.update(grad, 0.01)

if __name__ == '__main__':
    g = Game()
    g.play()
