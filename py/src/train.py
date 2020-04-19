import sys
import conf
from py.src.model import Net
from py.src.producer import Producer
import pickle
import random
from py.src.log_util import main_logger as logger


class Main(object):
    def __init__(self, is_continue=False):
        self.net = Net()
        self.is_continue = is_continue
        if self.is_continue:
            logger.info("Interrupt loading!")
            self.net.load(conf.model_path)
            with open(conf.data_path, "rb") as fp:
                self.data = pickle.load(fp)
            logger.info("load %s data!" % len(self.data))
            logger.info("Interrupt loaded!")
        else:
            self.data = []
        self.producer = Producer()
        self.game_num = 0

    def get_sample_data(self, n):
        mini_batch = [self.data.pop(random.randrange(len(self.data))) for _ in range(n)]
        return [item[0] for item in mini_batch], \
            [item[1] for item in mini_batch], \
            [item[2] for item in mini_batch]

    def run(self):
        try:
            if not self.is_continue:
                logger.info("Init data start!")
                self.data.extend(
                    self.producer.playn(conf.init_game_num, self.net)
                )
                self.game_num += conf.batch_game_num
                logger.info("Played %s games, produce %s data" % (self.game_num, len(self.data)))
            while True:
                batch_data = self.producer.playn(conf.batch_game_num, self.net)
                self.data.extend(batch_data)
                self.game_num += conf.batch_game_num
                logger.info("all games: %s, batch gamse: %s, this batch produce: %s" % (
                    self.game_num, conf.batch_game_num, len(batch_data)
                ))

                x, y1, y2 = self.get_sample_data(len(batch_data))
                self.net.train(x, y1, y2)
        except KeyboardInterrupt:
            with open(conf.data_path, "wb") as fwp:
                pickle.dump(self.data, fwp)
            self.net.save(conf.model_path)
            logger.info("Interrupt saved!")


if __name__ == "__main__":
    if len(sys.argv) == 2:
        if sys.argv[1] == "continue":
            m = Main(is_continue=True)
        else:
            raise RuntimeError("error param!")
    else:
        m = Main()
    m.run()
