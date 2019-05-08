from model import Net
from producer import Producer


class Main(object):
    def __init__(self):
        self.net = Net()
        self.producer = Producer()

    def run(self):
        while True:
            x, y1, y2 = self.producer.playn(2, self.net)
            if x:
                self.net.train(x, y1, y2)


if __name__ == "__main__":
    m = Main()
    m.run()
