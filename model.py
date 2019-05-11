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
import random
import numpy as np


def residual_block(x):
    t = Conv2D(filters=16, kernel_size=3, padding="same")(x)
    t = BatchNormalization()(t)
    t = Activation('relu')(t)
    t = Conv2D(filters=16, kernel_size=3, padding="same")(t)
    t = BatchNormalization()(t)
    t = Activation('relu')(t)
    return x + t


class Net(object):
    def __init__(self):
        inputs = Input(shape=(conf.board_size, conf.board_size, 3))

        x = Conv2D(filters=16, kernel_size=3, padding="same")(inputs)
        x = BatchNormalization()(x)
        x = Activation('relu')(x)

        for _ in range(conf.residual_blocks):
            x = residual_block(x)

        x = Flatten()(x)
        p = Dense(conf.num_outputs, activation="softmax")(x)
        v = Dense(1, activation='tanh')(x)
        net = Model(inputs=inputs, outputs=[p, v])
        net.compile(optimizer='Adam', loss=['categorical_crossentropy', 'mean_squared_error'])
        self.net = net

    def data_augmentation(self, xs, ys1, ys2):
        new_x, new_y1, new_y2 = [], [], []
        for x, y1, y2 in zip(xs, ys1, ys2):
            new_x.append(x)
            new_y1.append(y1)
            new_y2.append(y2)

            new_x.append(np.rot90(x))
            new_y1.append(np.rot90(y1.reshape((conf.board_size, conf.board_size))).reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(np.rot90(x, 2))
            new_y1.append(np.rot90(y1.reshape((conf.board_size, conf.board_size)), 2).reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(np.rot90(x, 3))
            new_y1.append(np.rot90(y1.reshape((conf.board_size, conf.board_size)), 3).reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(x[::-1, :, :])
            new_y1.append(y1.reshape((conf.board_size, conf.board_size))[::-1, :].reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(x[:, ::-1, :])
            new_y1.append(y1.reshape((conf.board_size, conf.board_size))[:, ::-1].reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(np.rot90(x)[::-1, :, :])
            new_y1.append(np.rot90(y1.reshape((conf.board_size, conf.board_size)))[::-1, :].reshape(conf.num_outputs))
            new_y2.append(y2)

            new_x.append(np.rot90(x)[:, ::-1, :])
            new_y1.append(np.rot90(y1.reshape((conf.board_size, conf.board_size)))[:, ::-1].reshape(conf.num_outputs))
            new_y2.append(y2)

        return new_x, new_y1, new_y2

    def train(self, x, y1, y2):
        x, y1, y2 = self.data_augmentation(x, y1, y2)
        sd = list(zip(x, y1, y2))
        random.shuffle(sd)
        x = [d[0] for d in sd]
        y1 = [d[1] for d in sd]
        y2 = [d[2] for d in sd]
        x = np.array(x, dtype=np.float32)
        y1 = np.array(y1, dtype=np.float32)
        y2 = np.array(y2, dtype=np.float32)
        self.net.fit(x=x, y=[y1, y2], batch_size=8, epochs=1)

    def predict(self, x):
        res = self.net.predict(x)
        return res[0][0], res[1][0][0]
