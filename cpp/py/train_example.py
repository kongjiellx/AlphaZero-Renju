# -*- coding: utf-8 -*-

import tensorflow as tf
from tensorflow.keras import optimizers, losses


class Model(tf.Module):
    def __init__(self):
        super(Model).__init__()
        input = tf.keras.layers.Input(
            shape=(2,),
            name="liuyekuan"
        )
        output = tf.keras.layers.Dense(1)(input)
        self.model = tf.keras.models.Model(inputs=input, outputs=output)
        self.model.build(input_shape=(None, 2))
        self.optimizer = optimizers.Adam()

    @tf.function(input_signature=[
        tf.TensorSpec(shape=(None, 2), dtype=tf.int32, name="x"),
        tf.TensorSpec(shape=None, dtype=tf.int32, name="y")
    ])
    def train_step(self, x, y):
        loss_func = losses.MeanSquaredError()
        with tf.GradientTape() as tape:
            predictions = self.model(x)
            loss = loss_func(y, predictions)
        gradients = tape.gradient(loss, self.model.trainable_variables)
        self.optimizer.apply_gradients(zip(gradients, self.model.trainable_variables))
        return gradients


def save():
    to_export = Model()
    tf.saved_model.save(
        to_export,
        '/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1',
        signatures={"train_step": to_export.train_step.get_concrete_function(
            tf.TensorSpec(shape=(None, 2), dtype=tf.int32, name="x"),
            tf.TensorSpec(shape=None, dtype=tf.int32, name="y")
        )}
    )


def load():
    model = tf.saved_model.load('/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1')
    print(model.signatures["train_step"](x=tf.constant([[1, 2], [3, 4]]), y=tf.constant([1, 2])))


if __name__ == "__main__":
    load()
    # m = Model()
    # # tf.saved_model.save(m, "/Users/admin/repos/AlphaZero-Renju/cpp/py/1")
    # tf.saved_model.save(m, "/home/liuyekuan/workspace/mine/AlphaZero-Renju/cpp/py/1",
    #                     signatures=m.train_model.get_concrete_function())
