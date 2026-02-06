# Generation Counter（Handle 系统的第二层）

## Day17 的致命漏洞：ABA
流程：
create → id=7
remove(7)
Registry erase 并复用 id=7
老 handle 仍然是 {7}

这就是 ABA：
A：对象 7
B：对象删除
A：新对象又是 7

## 工业标准解法：generation

## 练习任务（15min）
把 Day17 的 SafeRegistry 升级成：
Handle 带 gen
Entry 带 gen
remove() gen++
with() 校验 gen


## 总结
工程里的 handle 不是为了方便访问，而是为了隔离生命周期与并发复杂度。


## 还没做但已经理解
1. 为什么很多系统用 shared_ptr 而不是 valid flag
2. 为什么 handle 会是 64-bit packed（id + gen）
3. 为什么 lock-free 结构一定有 generation / epoch
