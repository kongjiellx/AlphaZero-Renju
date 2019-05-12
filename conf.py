# game rule
board_size = 5
win_num = 3
num_outputs = board_size ** 2

# model
model_path = 'model.params'
residual_blocks = 1
batch_size = 32
save_every_n_games = 10

# mcts
CPUCT = 0.2
explore_steps = 2
dirichlet_esp = 0.25
dirichlet_alpha = 0.03

