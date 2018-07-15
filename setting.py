board_size = 6
win_num = 3

model_path = 'model/renju.params'

num_batches = 220
learning_rate = 0.1
num_outputs = board_size ** 2
lr_period = 50
lr_decay = 0.2
weight_decay = 1e-4

batch_size = 4

CPUCT = 0.1