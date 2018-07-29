from model import ResNet
from setting import board_size
from mxnet import init
import mxnet as mx
from producer import Producer
from trainer import Trainer


class Main(object):
    def __init__(self):
        self.net = self.init_net()
        self.producer = Producer()
        self.trainer = Trainer()

    def init_net(self):
        net = ResNet(board_size ** 2)
        net.collect_params().initialize(init=init.Xavier(), ctx=mx.cpu())
        net.hybridize()
        return net

    def run(self):
        while True:
            x, y1, y2 = self.producer.playn(10, self.net)
            # x, y1, y2 = self.producer.play_a_agme(self.net, True)
            if x:
                self.trainer.train(self.net, x, y1, y2)


if __name__ == "__main__":
    m = Main()
    m.run()
