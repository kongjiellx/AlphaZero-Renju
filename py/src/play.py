#!/usr/bin/env python
# coding: utf-8

import pygame
import conf
from py.src import board
import numpy as np
from py.src.mcts import MCTS
from py.src.model import Net
from py.src.utils import idx2pos, pos2idx
import time

pygame.init()
pygame.display.set_caption('renju')
screen = pygame.display.set_mode((conf.background_size, conf.background_size), 0, 32)

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)


class Stone(board.Stone):
    def __init__(self, pos, player):
        super(Stone, self).__init__(pos, player)
        if player == board.Player.O:
            self.color = BLACK
        elif player == board.Player.X:
            self.color = WHITE
        self.coords = (conf.empty + self.pos[0] * conf.per_width, conf.empty + self.pos[1] * conf.per_width)

    def draw(self):
        pygame.draw.circle(screen, self.color, self.coords, conf.stone_radius, 0)
        pygame.display.update()


class Board(board.Board):
    def __init__(self):
        super(Board, self).__init__()

        background = pygame.image.load(conf.background).convert()
        self.outline = pygame.Rect(conf.empty, conf.empty, conf.game_area, conf.game_area)

        pygame.draw.rect(background, BLACK, self.outline, 3)
        self.outline.inflate_ip(20, 20)

        for i in range(conf.board_size - 1):
            for j in range(conf.board_size - 1):
                rect = pygame.Rect(conf.empty + (conf.per_width * i), conf.empty + (conf.per_width * j), conf.per_width,
                                   conf.per_width)
                pygame.draw.rect(background, BLACK, rect, 1)

        screen.blit(background, (0, 0))
        pygame.display.update()


def main():
    game_board = Board()
    mcts = MCTS()
    net = Net()
    net.load(conf.model_path)
    pygame.display.update()
    while True:
        if game_board.turn == board.Player.O:  # ai
            p = mcts.search(
                board=game_board,
                net=net,
                simulate_num=1000,
                T=0.1,
                add_dirichlet_noise=False
            )
            idx = np.random.choice(conf.board_size ** 2, p=p)
            pos = idx2pos(idx)
        else:  # human
            while True:
                flag = False
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        exit()
                    if event.type == pygame.MOUSEBUTTONDOWN:
                        if event.button == 1 and game_board.outline.collidepoint(event.pos):
                            x = int(round((event.pos[0] - conf.empty) / conf.per_width, 0))
                            y = int(round((event.pos[1] - conf.empty) / conf.per_width, 0))
                            pos = (x, y)
                            idx = pos2idx(pos)
                            flag = True
                            break
                if flag:
                    break
        mcts.change_root(idx)
        stone = Stone(pos, game_board.turn)
        is_done, winner = game_board.step(stone)
        stone.draw()

        if is_done:
            if winner == board.Player.O:
                print("O WIN!")
            elif winner == board.Player.X:
                print("X WIN!")
            else:
                print("NO WINNER!")
            time.sleep(1)
            break


if __name__ == "__main__":
    main()
