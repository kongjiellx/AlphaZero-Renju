from mxnet.gluon import nn
from mxnet import autograd
from mxnet import nd
from mxnet import gluon


class ResNet(nn.HybridBlock):
    def __init__(self, num_classes, verbose=False, **kwargs):
        super(ResNet, self).__init__(**kwargs)
        self.verbose = verbose
        with self.name_scope():
            net = self.net = nn.HybridSequential()
            net.add(nn.Conv2D(channels=16, kernel_size=3, strides=1))
            net.add(nn.BatchNorm())
            # net.add(nn.Activation(activation='relu'))
            # net.add(nn.Conv2D(channels=16, kernel_size=3, strides=1))
            # net.add(nn.BatchNorm())
            net.add(nn.Activation(activation='relu'))
            net.add(nn.Flatten())
            self.p = nn.Dense(num_classes)
            self.v = nn.Dense(1)

    def hybrid_forward(self, F, x):
        out = x
        for i, b in enumerate(self.net):
            out = b(out)
            if self.verbose:
                print('Block %d output: %s' % (i + 1, out.shape))
        return [self.p(out), F.tanh(self.v(out))]
