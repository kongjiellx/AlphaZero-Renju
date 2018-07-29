from setting import batch_size
from mxnet import autograd
from mxnet import gluon
from mxnet import nd
import numpy as np
import datetime
import random

class Trainer(object):
    def train(self, net, x, y1, y2):
        sd = list(zip(x, y1, y2))
        random.shuffle(sd)
        x = [d[0] for d in sd]
        y1 = [d[1] for d in sd]
        y2 = [d[2] for d in sd]
        x = np.array(x, dtype=np.float32)
        y1 = np.array(y1, dtype=np.float32)
        y2 = np.array(y2, dtype=np.float32)
        dataset_train = gluon.data.ArrayDataset(x, list(zip(y1, y2)))
        train_data = gluon.data.DataLoader(dataset_train, batch_size=batch_size, shuffle=True)
        trainer = gluon.Trainer(
            net.collect_params(), 'sgd', {"wd": 0.0001, "learning_rate": 0.01, "momentum": 0.9})
        softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss(sparse_label=False)
        l2loss = gluon.loss.L2Loss()
        # print(x, ps, v)
        for i in range(2):
            for data, label in train_data:
                with autograd.record():
                    output = net(data)
                    loss = softmax_cross_entropy(output[0], label[0]) + l2loss(output[1], label[1])
                loss.backward()
                trainer.step(batch_size)
                print("loss: %s" % nd.mean(loss).asscalar())
