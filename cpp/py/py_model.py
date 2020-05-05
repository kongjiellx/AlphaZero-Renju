# -*- coding: utf-8 -*-

from tensorflow.keras import optimizers, losses, metrics
import tensorflow as tf
from tensorflow.keras.models import Model
from tensorflow.keras.layers import (
    Input,
    Conv2D,
    BatchNormalization,
    Activation,
    Flatten,
    Dense,
    Add
)
from tensorflow.keras.regularizers import l2
from google.protobuf import text_format
from conf.conf_pb2 import Conf
import numpy as np


def load_conf():
    with open("conf/conf.pbtxt", "r") as fp:
        return text_format.Parse(fp.read(), Conf())


conf = load_conf()


def residual_block(x):
    t = Conv2D(filters=32, kernel_size=3, padding="same", kernel_regularizer=l2(conf.model_conf.l2_c), bias_regularizer=l2(conf.model_conf.l2_c))(x)
    t = BatchNormalization()(t)
    t = Activation('elu')(t)
    t = Conv2D(filters=32, kernel_size=3, padding="same", kernel_regularizer=l2(conf.model_conf.l2_c), bias_regularizer=l2(conf.model_conf.l2_c))(t)
    t = BatchNormalization()(t)
    add = Add()([x, t])
    return Activation('elu')(add)


class Model(tf.Module):
    def __init__(self):
        super(Model).__init__()
        inputs = Input(shape=(conf.game_conf.board_size, conf.game_conf.board_size, 3), dtype=tf.float32)

        x = Conv2D(
            filters=32,
            kernel_size=3,
            padding="same",
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(inputs)
        x = BatchNormalization()(x)
        x = Activation('elu')(x)

        for _ in range(conf.model_conf.residual_blocks):
            x = residual_block(x)

        ph = Conv2D(
            filters=2,
            kernel_size=1,
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(x)
        ph = BatchNormalization()(ph)
        ph = Activation('elu')(ph)
        ph = Flatten()(ph)
        ph = Dense(
            conf.game_conf.board_size * conf.game_conf.board_size,
            activation="softmax",
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(ph)

        vh = Conv2D(
            filters=1,
            kernel_size=1,
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(x)
        vh = BatchNormalization()(vh)
        vh = Activation('elu')(vh)
        vh = Flatten()(vh)
        vh = Dense(
            20,
            activation='elu',
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(vh)
        vh = Dense(
            1,
            activation='tanh',
            kernel_regularizer=l2(conf.model_conf.l2_c),
            bias_regularizer=l2(conf.model_conf.l2_c)
        )(vh)

        self.model = tf.keras.models.Model(inputs=inputs, outputs=[ph, vh])
        self.model.build(input_shape=(None, conf.game_conf.board_size, conf.game_conf.board_size, 3))
        self.optimizer = optimizers.Adam()

    @tf.function(input_signature=[
        tf.TensorSpec(shape=(None, conf.game_conf.board_size, conf.game_conf.board_size, 3), dtype=tf.float32, name="x"),
        tf.TensorSpec(shape=(None, conf.game_conf.board_size * conf.game_conf.board_size), dtype=tf.float32, name="p"),
        tf.TensorSpec(shape=None, dtype=tf.float32, name="v"),
    ])
    def train_step(self, x, p, v):
        p_loss_func = losses.CategoricalCrossentropy()
        v_loss_func = losses.MeanSquaredError()

        with tf.GradientTape() as tape:
            predictions = self.model(x)
            p_loss = p_loss_func(p, predictions[0])
            v_loss = v_loss_func(v, predictions[1])
            loss = p_loss + v_loss
        gradients = tape.gradient(loss, self.model.trainable_variables)
        self.optimizer.apply_gradients(zip(gradients, self.model.trainable_variables))
        return loss


def save():
    to_export = Model()
    tf.saved_model.save(
        to_export,
        conf.model_conf.model_path,
        signatures={
            "train_step": to_export.train_step.get_concrete_function(
                tf.TensorSpec(shape=(None, conf.game_conf.board_size, conf.game_conf.board_size, 3), dtype=tf.float32, name="x"),
                tf.TensorSpec(shape=(None, conf.game_conf.board_size * conf.game_conf.board_size), dtype=tf.float32, name="p"),
                tf.TensorSpec(shape=None, dtype=tf.float32, name="v"),
            )
        }
    )


def load():
    model = tf.saved_model.load(conf.model_conf.model_path,)
    for i in range(100):
        x = np.ones((1, 10, 10, 3)).tolist()
        y = [0.] * 100
        y[3] = 1.
        y = [y]
        print(model.signatures["train_step"](x=tf.constant(x), p=tf.constant(y), v=tf.constant(1.)))


if __name__ == "__main__":
    save()
    load()
