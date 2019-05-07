from model import Net
from setting import board_size
from producer import Producer
from trainer import Trainer


class Main(object):
    def __init__(self):
        self.net = Net()
        self.producer = Producer()
        self.trainer = Trainer()

    def run(self):
        while True:
            x, y1, y2 = self.producer.playn(10, self.net)
            if x:
                self.net.train(self.net, x, y1, y2)


if __name__ == "__main__":
    m = Main()
    m.run()
