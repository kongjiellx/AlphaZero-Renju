# -*- coding: utf-8 -*-

import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.keras.models import Model
from tensorflow.keras.layers import (
    Input,
    Conv2D,
    BatchNormalization,
    Activation,
    Flatten,
    Dense
)
import conf


class Net(object):
    def __init__(self):
        inputs = Input(shape=(conf.board_size, conf.board_size, 3))

        x = Conv2D(filters=16, kernel_size=3)(inputs)
        x = BatchNormalization()(x)
        x = Activation('tanh')(x)
        x = Flatten()(x)

        p = Dense(conf.num_outputs, activation="softmax")(x)
        v = Dense(1, activation='sigmoid')(x)
        net = Model(inputs=inputs, outputs=[p, v])
        net.compile(optimizer='Adam', loss=['categorical_crossentropy', 'mean_squared_error'])
        self.net = net


if __name__ == "__main__":
    import random
    import numpy as np
    net = Net()
    x = []
    y1 = []
    y2 = []
    for i in range(100):
        ax = np.zeros(shape=(conf.board_size, conf.board_size, 3))
        for i in range(conf.board_size):
            for j in range(conf.board_size):
                if random.randint(0, 1) == 0:
                    ax[i][j][2] = 0
                else:
                    ax[i][j][2] = 1
                a = random.randint(0, 10)
                if a == 0:
                    ax[i][j][0] = 1
                elif a == 1:
                    ax[i][j][1] = 1
        x.append(ax)
        y1.append([0] * conf.num_outputs)
        y2.append(random.randint(0, 1))

    net.net.fit(x=np.array(x), y=[np.array(y1), np.array(y2)])
