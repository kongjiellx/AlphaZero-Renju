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


class Net(object):
    def __init__(self):
        inputs = Input(shape=(conf.board_size, conf.board_size, 3))

        x = Conv2D(filters=16, kernel_size=3)(inputs)
        x = BatchNormalization()(x)
        x = Activation('tanh')(x)
        x = Flatten()(x)

        p = Dense(conf.num_outputs, activation="softmax")(x)
        v = Dense(1, activation='tanh')(x)
        net = Model(inputs=inputs, outputs=[p, v])
        net.compile(optimizer='Adam', loss=['categorical_crossentropy', 'mean_squared_error'])
        self.net = net

    def train(self, x, y1, y2):
        sd = list(zip(x, y1, y2))
        random.shuffle(sd)
        x = [d[0] for d in sd]
        y1 = [d[1] for d in sd]
        y2 = [d[2] for d in sd]
        x = np.array(x, dtype=np.float32)
        y1 = np.array(y1, dtype=np.float32)
        y2 = np.array(y2, dtype=np.float32)
        self.net.fit(x=x, y=[y1, y2], epochs=2)

    def predict(self, x):
        res = self.net.predict(x)
        return res[0][0], res[1][0][0]
