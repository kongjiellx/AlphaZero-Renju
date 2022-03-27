#!/usr/bin/env python
# coding: utf-8

import pygame
from enum import Enum
import time

board_size = 13
background_size = 630
game_area = 540
background = "/Users/admin/fish/code/AlphaZero-Renju/py/images/small.jpg"
empty = int((background_size - game_area) / 2)
per_width = int(game_area / (board_size - 1))
stone_radius = int(per_width / 2)

pygame.init()
pygame.display.set_caption('AlphaZero-Renju')
screen = pygame.display.set_mode((background_size, background_size), 0, 32)

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

class Player(Enum):
    O = 1
    X = 2


class Stone:
    def __init__(self, pos, player):
        if player == Player.O:
            self.color = BLACK
        elif player == Player.X:
            self.color = WHITE
        self.coords = (empty + pos[0] * per_width, empty + pos[1] * per_width)

    def draw(self):
        pygame.draw.circle(screen, self.color, self.coords, stone_radius, 0)


class Board:
    def __init__(self, background):
        background = pygame.image.load(background).convert()
        self.outline = pygame.Rect(per_width, per_width, game_area, game_area)

        pygame.draw.rect(background, BLACK, self.outline, 3)
        self.outline.inflate_ip(20, 20)

        for i in range(board_size - 1):
            for j in range(board_size - 1):
                rect = pygame.Rect(empty + (per_width * i), empty + (per_width * j), per_width, per_width)
                pygame.draw.rect(background, BLACK, rect, 1)

        screen.blit(background, (0, 0))


if __name__ == "__main__":
    game_board = Board(background)
    pygame.display.update()
    current_player = Player.O
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1 and game_board.outline.collidepoint(event.pos):
                    x = int(round((event.pos[0] - empty) / per_width, 0))
                    y = int(round((event.pos[1] - empty) / per_width, 0))
                    pos = (x, y)
                    stone = Stone(pos, current_player)
                    stone.draw()
                    if current_player == Player.O:
                        current_player = Player.X
                    else:
                        current_player = Player.O
                    pygame.display.update()
            time.sleep(0.1)
