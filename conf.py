# game rule
board_size = 5
win_num = 3
num_outputs = board_size ** 2

# model
model_path = 'params.h5'
residual_blocks = 2
batch_size = 32
save_every_n_games = 100

# mcts
CPUCT = 200
explore_steps = 2
dirichlet_esp = 0.25
dirichlet_alpha = 0.03

