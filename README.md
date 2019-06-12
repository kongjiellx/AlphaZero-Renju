## AlphaZero-Renju
### References：
1. [A blog explain how mcts works](http://tim.hibal.org/blog/alpha-zero-how-and-why-it-works/)
2. [AlphaZero connect four](https://medium.com/oracledevs/lessons-from-implementing-alphazero-7e36e9054191), there are someting interesting ideas, like use Q to instead of Z to be training target.
3. [Some notices about mcts](https://medium.com/oracledevs/lessons-from-implementing-alphazero-7e36e9054191)
4. [tensorflow AGZ implement(NOT an official version of AlphaGo)](https://github.com/tensorflow/minigo)
5. [A renju program follow AlphaZero. ](https://github.com/junxiaosong/AlphaZero_Gomoku) [Here](https://zhuanlan.zhihu.com/p/32089487) is a chinese doc about it. I think this implement has some bugs. For example the author compute final z by mistake, but when do backup he add a negative sign, so maybe the result is ok.

### Requirements
- python 3.5+
- tensorflow==2.0.0a0
- loguru==0.2.5
- pygame==1.9.6 (needed by play.py, training doesn't need this.)

### Results
- Training about 24 hours, each move with 100 mcts simulations:
![renju](https://raw.githubusercontent.com/kongjiellx/AlphaZero-Renju/master/523.gif)

- Training about 48 hours, each move with 400 mcts simulations:
![renju](https://raw.githubusercontent.com/kongjiellx/AlphaZero-Renju/master/525.gif)
