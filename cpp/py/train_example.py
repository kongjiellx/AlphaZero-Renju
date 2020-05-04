# -*- coding: utf-8 -*-

import tensorflow as tf
from tensorflow.keras import models, layers, optimizers, losses, metrics

import numpy as np


class Model(tf.Module):
    def __init__(self):
        input = tf.keras.layers.Input(
            shape=(2,),
            name="liuyekuan"
        )
        output = tf.keras.layers.Dense(1)(input)
        self.model = tf.keras.models.Model(inputs=input, outputs=output)
        self.model.build(input_shape=(None, 2))

        self.optimizer = optimizers.Adam()
        self.loss_func = losses.MeanSquaredError()

        self.train_loss = tf.keras.metrics.Mean(name='train_loss')
        self.train_metric = tf.keras.metrics.MeanAbsoluteError(name='train_mae')

        self.valid_loss = tf.keras.metrics.Mean(name='valid_loss')
        self.valid_metric = tf.keras.metrics.MeanAbsoluteError(name='valid_mae')

    @tf.function(input_signature=[
        tf.TensorSpec(shape=(None, 2), dtype=tf.int32),
        tf.TensorSpec(shape=None, dtype=tf.int32)
    ])
    def train_step(self, features, labels):
        with tf.GradientTape() as tape:
            predictions = self.model(features)
            loss = self.loss_func(labels, predictions)
        gradients = tape.gradient(loss, self.model.trainable_variables)
        self.optimizer.apply_gradients(zip(gradients, self.model.trainable_variables))

        self.train_loss.update_state(loss)
        self.train_metric.update_state(labels, predictions)
        tf.print(self.train_loss.result())

    @tf.function
    def valid_step(self, features, labels):
        predictions = self.model(features)
        batch_loss = self.loss_func(labels, predictions)
        self.valid_loss.update_state(batch_loss)
        self.valid_metric.update_state(labels, predictions)
        tf.print(self.valid_metric.result())

    @tf.function
    def train_model(self):
        self.train_step(self.model, np.array([[1, 2], [3, 4]]), [1, 2])
        self.valid_step(self.model, np.array([[1, 2], [3, 4]]), [1, 2])

    def __call__(self):
        print(dir(self.train_step))
        print(self.train_step.get_concrete_function(
            tf.TensorSpec(shape=(None, 2), dtype=tf.int32),
            tf.TensorSpec(shape=None, dtype=tf.int32)
        ).graph)


if __name__ == "__main__":
    m = Model()
    tf.saved_model.save(m, "/Users/admin/repos/AlphaZero-Renju/cpp/py/1")
