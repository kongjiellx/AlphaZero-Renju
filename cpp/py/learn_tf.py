# -*- coding: utf-8 -*-

import tensorflow as tf
from tensorflow import keras
import numpy as np


def save_model(save_path):
    input = keras.layers.Input(
        shape=(2,),
        name="liuyekuan"
    )

    output = keras.layers.Dense(1)(input)

    net = keras.models.Model(inputs=input, outputs=output)
    net.build(input_shape=(None, 2))

    # net.compile(
    #     optimizer=keras.optimizers.SGD(lr=0, momentum=0.9),
    #     loss="mean_squared_error"
    # )
    # net.fit(x=np.array([[1, 2], [3, 4]]), y=np.array([0, 1]))
    # print(net.predict(np.array([[1, 2]])))
    # tf.saved_model.save(net, save_path)


def load_model(path):
    model = tf.saved_model.load(path, tags=[tf.saved_model.TRAINING])
    print(dir(model))
    print(model.signatures["serving_default"](tf.constant([[1., 2.]])))


if __name__ == "__main__":
    save_model("/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1")
    # load_model("/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1")
