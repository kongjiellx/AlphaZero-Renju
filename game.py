from setting import *
import numpy as np


class Game(object):
    def __init__(self):
        self.board = Board()

    def run(self):
        while True:
            ipt = input("pos:")
            pos = (int(ipt.strip().split(',')[0]), int(ipt.strip().split(',')[1]))
            stone = Stone(pos, self.board.turn)
            self.board.do_turn()
            self.board.add_stone(stone)
            self.board.paint()
            if self.board.is_over(stone) == Color.black:
                print("BLACK WIN!")
            elif self.board.is_over(stone) == Color.white:
                print("WHITE WIN!")


class Color:
    black = -1
    white = 1


class Stone(object):
    def __init__(self, pos, color):
        self.pos = pos
        self.color = color


class Board(object):
    def __init__(self):
        self.size = board_size
        self.win_num = win_num
        self.turn = Color.black
        self.board = self._init_board()

    def _init_board(self):
        ret = []
        for i in range(self.size):
            row = []
            for j in range(self.size):
                row.append(0)
            ret.append(row)
        return ret

    def do_turn(self):
        if self.turn == Color.black:
            self.turn = Color.white
        else:
            self.turn = Color.black

    def get_feature(self):
        ret = np.zeros(shape=(3, board_size, board_size))
        for i in range(board_size):
            for j in range(board_size):
                ret[2][i][j] = self.turn
                if self.board[i][j] == Color.black:
                    ret[0][i][j] = 1
                elif self.board[i][j] == Color.white:
                    ret[1][i][j] = 1
        return ret.reshape((1, 3, board_size, board_size))

    def is_leagal(self, stone):
        if stone.color == self.turn and self.board[stone.pos[0]][stone.pos[1]] == 0:
            return True
        else:
            return False

    def add_stone(self, stone):
        if self.board[stone.pos[0]][stone.pos[1]] != 0:
            raise BaseException
        else:
            self.board[stone.pos[0]][stone.pos[1]] = stone.color

    def paint(self):
        strs = ''
        for i in range(self.size):
            row = []
            for j in range(self.size):
                if self.board[i][j] == Color.black:
                    row.append('O')
                elif self.board[i][j] == Color.white:
                    row.append('X')
                else:
                    row.append('-')
            strs += ' '.join(row) + '\n'
        print(strs)

    def is_over(self, last_stone):
        num = 0
        for i in range(self.size):
            if self.board[last_stone.pos[0]][i] == last_stone.color:
                num += 1
                if num >= self.win_num:
                    return last_stone.color
            else:
                num = 0

        num = 0
        for i in range(self.size):
            if self.board[i][last_stone.pos[1]] == last_stone.color:
                num += 1
                if num >= self.win_num:
                    return last_stone.color
            else:
                num = 0

        num = 0
        i = last_stone.pos[0] - min(last_stone.pos)
        j = last_stone.pos[1] - min(last_stone.pos)
        while i < self.size and j < self.size:
            if self.board[i][j] == last_stone.color:
                num += 1
                if num >= self.win_num:
                    return last_stone.color
            else:
                num = 0
            i += 1
            j += 1

        num = 0
        i = last_stone.pos[0] - min(last_stone.pos)
        j = last_stone.pos[1] - min(last_stone.pos)
        while i < self.size and j > 0:
            if self.board[i][j] == last_stone.color:
                num += 1
                if num >= self.win_num:
                    return last_stone.color
            else:
                num = 0
            i += 1
            j -= 1

        for i in range(self.size):
            for j in range(self.size):
                if self.board[i][j] == 0:
                    return 0
        return 2


if __name__ == "__main__":
    game = Game()
    game.run()
