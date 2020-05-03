# game rule
board_size = 10
win_num = 5
num_outputs = board_size ** 2

# model
model_path = '/Users/admin/repos/AlphaZero-Renju/py/model/params.h5'
residual_blocks = 1
batch_size = 32
save_every_n_games = 100
l2_c = 1e-4
lr = 0.01
validation_split = 0.2

# mcts
simulate_num = 100
CPUCT = 5
explore_steps = 4
dirichlet_esp = 0.25
dirichlet_alpha = 1

# main
init_game_num = 200
batch_game_num = 20
data_path = 'data.pkl'

# ui
background_size = 630
game_area = 540
background = 'py/images/small.jpg'
empty = int((background_size - game_area) / 2)
per_width = int(game_area / (board_size - 1))
stone_radius = int(per_width / 2)
