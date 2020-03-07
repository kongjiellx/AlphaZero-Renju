# -*- coding: utf-8 -*-

import tensorflow as tf
from conf.conf_pb2 import Conf
from google.protobuf import text_format
from tensorflow import keras
from tensorflow.keras.regularizers import l2
from tensorflow.python.keras import backend as K


def load_conf(path):
    with open(path, "r") as fp:
        return text_format.Parse(fp.read(), Conf())


def residual_block(conf, x):
    t = keras.layers.Conv2D(
        filters=32,
        kernel_size=3,
        padding="same",
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(x)
    t = keras.layers.BatchNormalization()(t)
    t = keras.layers.Activation('elu')(t)
    t = keras.layers.Conv2D(
        filters=32,
        kernel_size=3,
        padding="same",
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(t)
    t = keras.layers.BatchNormalization()(t)
    add = keras.layers.Add()([x, t])
    return keras.layers.Activation('elu')(add)


def build_model(conf, save_path):
    inputs = keras.layers.Input(
        shape=(conf.game_conf.board_size, conf.game_conf.board_size, 3)
    )

    x = keras.layers.Conv2D(
        filters=32,
        kernel_size=3,
        padding="same",
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(inputs)
    x = keras.layers.BatchNormalization()(x)
    x = keras.layers.Activation('elu')(x)

    for _ in range(conf.model_conf.residual_blocks):
        x = residual_block(conf, x)

    ph = keras.layers.Conv2D(
        filters=2,
        kernel_size=1,
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(x)
    ph = keras.layers.BatchNormalization()(ph)
    ph = keras.layers.Activation('elu')(ph)
    ph = keras.layers.Flatten()(ph)
    ph = keras.layers.Dense(
        conf.game_conf.board_size ** 2,
        activation="softmax",
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(ph)

    vh = keras.layers.Conv2D(
        filters=1,
        kernel_size=1,
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(x)
    vh = keras.layers.BatchNormalization()(vh)
    vh = keras.layers.Activation('elu')(vh)
    vh = keras.layers.Flatten()(vh)
    vh = keras.layers.Dense(
        20,
        activation='elu',
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(vh)
    vh = keras.layers.Dense(
        1, activation='tanh',
        kernel_regularizer=l2(conf.model_conf.l2_c),
        bias_regularizer=l2(conf.model_conf.l2_c)
    )(vh)

    net = keras.models.Model(inputs=inputs, outputs=[ph, vh])
    net.compile(
        optimizer=keras.optimizers.SGD(lr=conf.model_conf.lr, momentum=0.9),
        loss=["categorical_crossentropy", "mean_squared_error"])

    tf.io.write_graph(K.get_session().graph.as_graph_def(), save_path, "model", as_text=True)


if __name__ == "__main__":
    conf = load_conf("conf/conf.pbtxt")
    build_model(conf, "/Users/admin/repos/AlphaZero-Renju/cpp/conf/graph")
