from mxnet import autograd
from mxnet import gluon
from mxnet import init
from mxnet import nd
import numpy as np
import utils
from model import ResNet
import datetime
from setting import *

def train(net, train_data, valid_data, num_epochs, lr, wd, ctx, lr_period, lr_decay):
    trainer = gluon.Trainer(
            net.collect_params(), 'sgd', {'learning_rate': lr, 'momentum': 0.9, 'wd': wd})

    softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()

    prev_time = datetime.datetime.now()
    for batch in range(num_batches):
        train_loss = 0.0
        train_acc = 0.0
        if batch > 0 and batch % lr_period == 0:
            trainer.set_learning_rate(trainer.learning_rate * lr_decay)
        num = 0
        for data, label in train_data:
            label = label.as_in_context(ctx)
            with autograd.record():
                output = net(data.as_in_context(ctx))
                loss = softmax_cross_entropy(output, label)
            loss.backward()
            trainer.step(batch_size)
            train_loss += nd.mean(loss).asscalar()
            train_acc += utils.accuracy(output, label)
            num += 1
            if num % 1000 == 0:
                print(num)

        cur_time = datetime.datetime.now()
        h, remainder = divmod((cur_time - prev_time).seconds, 3600)
        m, s = divmod(remainder, 60)
        time_str = "Time %02d:%02d:%02d" % (h, m, s)
        if valid_data is not None:
            valid_acc = utils.evaluate_accuracy(valid_data, net, ctx)
            epoch_str = ("Epoch %d. Loss: %f, Train acc %f, Valid acc %f, "
                         % (epoch, train_loss / len(train_data),
                            train_acc / len(train_data), valid_acc))
        else:
            epoch_str = ("Epoch %d. Loss: %f, Train acc %f, "
                         % (epoch, train_loss / len(train_data),
                            train_acc / len(train_data)))
        prev_time = cur_time
        print(epoch_str + time_str + ', lr ' + str(trainer.learning_rate))
        net.save_params(model_path)

if __name__ == '__main__':

    ctx = utils.try_gpu()
    net = ResNet(num_outputs)
    net.initialize(ctx=ctx, init=init.Xavier())
    net.hybridize()
    train(net, train_data, valid_data, num_epochs, learning_rate,
            weight_decay, ctx, lr_period, lr_decay)
    net.save_params(model_path)


