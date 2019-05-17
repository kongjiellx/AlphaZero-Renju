import sys
import conf
from model import Net
from producer import Producer


class Main(object):
    def __init__(self, is_continue=False):
        self.net = Net()
        if is_continue:
            self.net.load(conf.model_path)
        self.producer = Producer()
        self.game_num = 0

    def run(self):
        while True:
            self.producer.playn(5, self.net)
            self.game_num += 5
            if len(self.producer.data) > conf.sample_num:
                x, y1, y2 = self.producer.get_sample_data(conf.sample_num)
                self.net.train(x, y1, y2)
            if self.game_num % conf.save_every_n_games == 0:
                self.net.save(conf.model_path)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        if sys.argv[1] == "continue":
            m = Main(True)
        else:
            raise RuntimeError("error param!")
    else:
        m = Main()
    m.run()
