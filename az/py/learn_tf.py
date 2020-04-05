# -*- coding: utf-8 -*-

import tensorflow as tf
from tensorflow import keras
from tensorflow.python.keras import backend as K
import numpy as np

def build_keras_model(save_path):
    input = keras.layers.Input(
        shape=(2,),
        name="liuyekuan"
    )

    output = keras.layers.Dense(1)(input)

    net = keras.models.Model(inputs=input, outputs=output)
    net.compile(
        optimizer=keras.optimizers.SGD(lr=0, momentum=0.9),
        loss="mean_squared_error"
    )
    net.fit(x=np.array([[1, 2], [3, 4]]), y=[0, 1])
    tf.io.write_graph(K.get_session().graph.as_graph_def(), save_path, "test0.pbtxt", as_text=True)


def build_tf_model(path):
    # Batch of input and target output (1x1 matrices)
    x = tf.placeholder(tf.float32, shape=[None, 1, 1], name='input')
    y = tf.placeholder(tf.float32, shape=[None, 1, 1], name='target')

    # Trivial linear model
    y_ = tf.identity(tf.layers.dense(x, 1), name='output')

    # Optimize loss
    loss = tf.reduce_mean(tf.square(y_ - y), name='loss')
    optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.01)
    train_op = optimizer.minimize(loss, name='train')

    init = tf.global_variables_initializer()

    # tf.train.Saver.__init__ adds operations to the graph to save
    # and restore variables.
    saver_def = tf.train.Saver().as_saver_def()

    print('Run this operation to initialize variables     : ', init.name)
    print('Run this operation for a train step            : ', train_op.name)
    print('Feed this tensor to set the checkpoint filename: ', saver_def.filename_tensor_name)
    print('Run this operation to save a checkpoint        : ', saver_def.save_tensor_name)
    print('Run this operation to restore a checkpoint     : ', saver_def.restore_op_name)

    tf.io.write_graph(tf.get_default_graph().as_graph_def(), path, "test_tf.pbtxt", as_text=True)


if __name__ == "__main__":
    build_keras_model("/Users/admin/repos/AlphaZero-Renju/cpp/conf")
    # build_tf_model("/Users/admin/repos/AlphaZero-Renju/cpp/conf")
