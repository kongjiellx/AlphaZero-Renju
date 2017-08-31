import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import numpy as np
import pickle


class Model(object):
    def __init__(self):
        self.W = np.random.rand(25, 25)
        self.b = np.zeros((25,))

    def softmax(self, x):
        return np.exp(x) / (np.sum(np.exp(x)))

    def forward(self, x):
        return self.softmax(np.dot(self.W, x) + self.b)

    def backward(self, reward, X, y, y_):
        return {'W': reward * np.dot(np.array(X).T, (y - y_)), 'b': reward* np.sum(y - y_)}

    def update(self, grad, lr):
        self.W += grad['W'] * lr
        self.b += grad['b'] * lr

    def save(self, path='model.bin'):
        fp = open(path, 'wb')
        pickle.dump({'W': self.W, 'b': self.b}, fp)
        fp.close()

    def load(self, path='model.bin'):
        fp = open(path, 'rb')
        param = pickle.load(fp)
        fp.close()
        self.W = param['W']
        self.b = param['b']


