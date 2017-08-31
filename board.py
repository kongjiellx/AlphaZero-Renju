import numpy as np

SIZE = 5
BLACK = 1
WHITE = 2


class Board(object):
    def __init__(self):
        self.board = np.zeros((SIZE, SIZE))
        self.color = BLACK

    def step(self, position):
        """
        :param position:
        :return: done, reward, observation
        """
        legal = self.check(position)
        if not legal:
            return True, -1, self.board
        else:
            self.board[position] = self.color
            self.turn()
            return self.finish(position), self.calculate_reward(position), self.board

    def turn(self):
        if self.color == BLACK:
            self.color = WHITE
        else:
            self.color = BLACK

    def check(self, position):
        if self.board[position] == 0:
            return True
        else:
            return False

    def same(self, points):
        try:
            for point in points:
                if self.board[point] != BLACK:
                    return False
            return True
        except:
            return False

    def finish(self, position):
        if self.same([position, (position[0], position[1] - 1), (position[0], position[1] - 2)]) \
                or self.same([position, (position[0], position[1] + 1), (position[0], position[1] + 2)]) \
                or self.same([position, (position[0] - 1, position[1]), (position[0] - 2, position[1])]) \
                or self.same([position, (position[0] + 1, position[1]), (position[0] + 2, position[1])]) \
                or self.same([position, (position[0] - 1, position[1] - 1), (position[0] - 2, position[1] - 2)]) \
                or self.same([position, (position[0] + 1, position[1] + 1), (position[0] + 2, position[1] + 2)]) \
                or self.same([position, (position[0] - 1, position[1] + 1), (position[0] - 2, position[1] + 2)]) \
                or self.same([position, (position[0] + 1, position[1] - 1), (position[0] + 2, position[1] - 2)]) \
                or self.same([position, (position[0] - 1, position[1]), (position[0] + 1, position[1])]) \
                or self.same([position, (position[0], position[1] - 1), (position[0], position[1] + 1)]) \
                or self.same([position, (position[0] - 1, position[1] - 1), (position[0] + 1, position[1] + 1)]) \
                or self.same([position, (position[0] - 1, position[1] + 1), (position[0] + 1, position[1] - 1)]):
            return True
        if np.min(self.board) != 0:
            return True
        else:
            return False

    def calculate_reward(self, position):
        if self.same([position, (position[0], position[1] - 1), (position[0], position[1] - 2)]) \
                or self.same([position, (position[0], position[1] + 1), (position[0], position[1] + 2)]) \
                or self.same([position, (position[0] - 1, position[1]), (position[0] - 2, position[1])]) \
                or self.same([position, (position[0] + 1, position[1]), (position[0] + 2, position[1])]) \
                or self.same([position, (position[0] - 1, position[1] - 1), (position[0] - 2, position[1] - 2)]) \
                or self.same([position, (position[0] + 1, position[1] + 1), (position[0] + 2, position[1] + 2)]) \
                or self.same([position, (position[0] - 1, position[1] + 1), (position[0] - 2, position[1] + 2)]) \
                or self.same([position, (position[0] + 1, position[1] - 1), (position[0] + 2, position[1] - 2)]) \
                or self.same([position, (position[0] - 1, position[1]), (position[0] + 1, position[1])]) \
                or self.same([position, (position[0], position[1] - 1), (position[0], position[1] + 1)]) \
                or self.same([position, (position[0] - 1, position[1] - 1), (position[0] + 1, position[1] + 1)]) \
                or self.same([position, (position[0] - 1, position[1] + 1), (position[0] + 1, position[1] - 1)]):
            return 1
        elif self.same([position, (position[0] - 1, position[1])])\
                or self.same([position, (position[0] + 1, position[1])]) \
                or self.same([position, (position[0], position[1] - 1)]) \
                or self.same([position, (position[0], position[1] + 1)]) \
                or self.same([position, (position[0] - 1, position[1] - 1)]) \
                or self.same([position, (position[0] - 1, position[1] + 1)]) \
                or self.same([position, (position[0] + 1, position[1] - 1)]) \
                or self.same([position, (position[0] + 1, position[1] + 1)]):
            return 0.1
        else:
            return 0

    def get_x(self):
        return self.board.reshape((SIZE ** 2,))

