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
from tensorflow.keras.regularizers import l2
from tensorflow.keras.callbacks import EarlyStopping
import conf
import numpy as np


def residual_block(x):
    t = Conv2D(filters=32, kernel_size=3, padding="same", kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(x)
    t = BatchNormalization()(t)
    t = Activation('elu')(t)
    t = Conv2D(filters=32, kernel_size=3, padding="same", kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(t)
    t = BatchNormalization()(t)
    return Activation('elu')(x + t)


class Net(object):
    def __init__(self):
        inputs = Input(shape=(conf.board_size, conf.board_size, 3))

        x = Conv2D(filters=32, kernel_size=3, padding="same", kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(inputs)
        x = BatchNormalization()(x)
        x = Activation('elu')(x)

        for _ in range(conf.residual_blocks):
            x = residual_block(x)

        ph = Conv2D(filters=2, kernel_size=1, kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(x)
        ph = BatchNormalization()(ph)
        ph = Activation('elu')(ph)
        ph = Flatten()(ph)
        ph = Dense(conf.num_outputs, activation="softmax", kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(ph)

        vh = Conv2D(filters=1, kernel_size=1, kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(x)
        vh = BatchNormalization()(vh)
        vh = Activation('elu')(vh)
        vh = Flatten()(vh)
        vh = Dense(20, activation='elu', kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(vh)
        vh = Dense(1, activation='tanh', kernel_regularizer=l2(conf.l2_c), bias_regularizer=l2(conf.l2_c))(vh)

        net = Model(inputs=inputs, outputs=[ph, vh])
        net.compile(optimizer=keras.optimizers.SGD(lr=conf.lr, momentum=0.9), loss=["categorical_crossentropy", "mean_squared_error"])
        self.net = net


    def train(self, x, y1, y2):
        x = np.array(x, dtype=np.float32)
        y1 = np.array(y1, dtype=np.float32)
        y2 = np.array(y2, dtype=np.float32)
        self.net.fit(
            x=x,
            y=[y1, y2],
            batch_size=conf.batch_size,
            epochs=100,
            validation_split=conf.validation_split,
            callbacks=[EarlyStopping()],
        )

    def predict(self, x):
        res = self.net.predict(x)
        return res[0][0], res[1][0][0]

    def save(self, path):
        self.net.save_weights(path)

    def load(self, path):
        self.net.load_weights(path)
