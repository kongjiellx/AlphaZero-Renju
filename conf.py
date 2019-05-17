# game rule
board_size = 5
win_num = 3
num_outputs = board_size ** 2

# model
model_path = 'params.h5'
residual_blocks = 1
batch_size = 32
save_every_n_games = 100
l2_c = 1e-4
lr = 0.001
validation_split = 0.2

# mcts
simulate_num = 100
CPUCT = 5
explore_steps = 4
dirichlet_esp = 0.25
dirichlet_alpha = 0.03

# producer
maxlen = 20000
sample_num = 2000
