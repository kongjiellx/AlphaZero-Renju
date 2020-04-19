# -*- coding: utf-8 -*-

import conf

def pos2idx(pos):
    return pos[0] * conf.board_size + pos[1]

def idx2pos(idx):
    return (idx // conf.board_size, idx % conf.board_size)
