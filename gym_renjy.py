# -*- coding: utf-8 -*-

# Imports
import gym
import time
# Change import on installed on pip
import sys, os
import gym_renju
env = gym.make('Renju15x15-v0') # default 'beginner' level opponent policy

env.reset()
env.render(mode='human')

# Play a game
env.reset()
for _ in range(225):
    action = env.action_space.sample()
    observation, reward, done, info = env.step(action)
    print(observation, reward, done, info)
    env.render(mode='human')
    if done:
        print ("Game is Over")
        break
