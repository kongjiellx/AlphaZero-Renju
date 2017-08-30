import numpy as np

SIZE = 5
BLACK = 1
WHITE = 2

class Board(object):
    def __init__(self):
        self.board = np.zeros((SIZE, SIZE))
        self.color = BLACK

    def step(self, position):
        if not self.check(position):
            return
        else:
            self.board[position] = self.color
            self.turn()

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

    def getX(self):
        return self.board

