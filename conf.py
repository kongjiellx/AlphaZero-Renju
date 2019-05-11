# game rule
board_size = 5
win_num = 3
num_outputs = board_size ** 2

# model
model_path = 'model/renju.params'
residual_blocks = 2
batch_size = 32

# mcts
CPUCT = 0.2
explore_steps = 5
dirichlet_esp = 0.25
dirichlet_alpha = 0.03

