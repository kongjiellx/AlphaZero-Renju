from board import Board, Player, Stone
from mcts import MCTS
import conf
import numpy as np
import random
from log_util import producer_logger as logger


class Producer(object):
    def data_augmentation(self, data):
        new_data = []
        for x, y1, y2 in data:
            for i in [1, 2, 3, 4]:
                new_data.append((
                    np.rot90(x, i),
                    np.rot90(y1.reshape((conf.board_size, conf.board_size)), i).reshape(conf.num_outputs),
                    y2
                ))
                new_data.append((
                    x[::-1, :, :],
                    y1.reshape((conf.board_size, conf.board_size))[::-1, :].reshape(conf.num_outputs),
                    y2
                ))
        return new_data

    def playn(self, n, net):
        data = []
        for i in range(n):
            data.extend(self.play_a_agme(net, True))
        return data

    def play_a_agme(self, net, paint=False):
        logger.info("New game start!")
        steps = 0
        board = Board()
        mcts = MCTS()
        data = []
        ps = []
        while True:
            feature = board.get_feature()
            p = mcts.search(
                board=board,
                net=net,
                simulate_num=conf.simulate_num,
                T=1 if steps < conf.explore_steps else 0.5,
                add_dirichlet_noise=True,
            )

            debug_ps, debug_v = net.predict(np.array([feature]))
            logger.info("V: %s" % debug_v)
            info = ''
            info += "NetWork predict probs:\n"
            for i in range(1, len(debug_ps) + 1):
                info += "%.3f" % debug_ps[i - 1] + ' '
                if i % conf.board_size == 0:
                    info += '\n'
            logger.info(info)

            info = ''
            info += "MCTS search probs:\n"
            for i in range(1, len(p) + 1):
                info += "%.3f" % p[i - 1] + ' '
                if i % conf.board_size == 0:
                    info += '\n'
            logger.info(info)

            data.append(feature)
            ps.append(p)

            idx = np.random.choice(conf.board_size ** 2, p=p)
            pos = (idx // conf.board_size, idx % conf.board_size)
            mcts.change_root(idx)
            stone = Stone(pos, board.turn)
            is_done, winner = board.step(stone)
            steps += 1
            if paint:
                logger.info(board)
            if is_done:
                if paint:
                    if winner == Player.O:
                        logger.info("O WIN!")
                    elif winner == Player.X:
                        logger.info("X WIN!")
                    else:
                        logger.info("NO WINNER!")
                if winner == Player.O:
                    vs = [Player.O] * len(data)
                elif winner == Player.X:
                    vs = [Player.X] * len(data)
                else:
                    vs = [0] * len(data)
                return self.data_augmentation(
                    zip(data, ps, vs)
                )
