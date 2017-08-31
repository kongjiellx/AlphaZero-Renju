import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import numpy as np


class Model(object):
    def __init__(self):
        self.W = np.random.rand(25, 25)
        self.b = np.zeros((25,))

    def softmax(self, x):
        return np.exp(x) / (np.sum(np.exp(x)))

    def forward(self, x):
        return self.softmax(np.dot(self.W, x) + self.b)

    def backward(self, reward, y, y_):
        return np.dot(reward.T, y - y_)

    def update(self, grad, lr):
        self.W += grad['W'] * lr
        self.b += grad['b'] * lr



