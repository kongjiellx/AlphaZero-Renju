## AlphaGoZero-Renju
References：
1. A blog explain how mcts works [1](http://tim.hibal.org/blog/alpha-zero-how-and-why-it-works/)
2. https://medium.com/oracledevs/lessons-from-implementing-alphazero-7e36e9054191 AlphaZero connect four，提了一些有意思的思考，比如神经网络拟合Z变成拟合Q
3. https://zhuanlan.zhihu.com/p/25345778 mcts提了几个值得注意的点
4. https://github.com/tensorflow/minigo tf项目下面一个谷歌大佬自己的实现（不是官方开源，但是我觉得都挂tf下面了应该不会有bug啥的了吧）
5. https://github.com/junxiaosong/AlphaZero_Gomoku （https://zhuanlan.zhihu.com/p/32089487 这篇文章对应的代码，但是似乎存在一些bug。比如我认为终局时候返回的z他弄反了。虽然backup的时候他又加了个负号看起来不影响结果。。）

训练大概24小时的结果:
- Each move with 100 MCTS playouts:
![renju](https://raw.githubusercontent.com/kongjiellx/AlphaGoZero-like-Renju/master/renju.gif)
