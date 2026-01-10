# Topics on std::move

## 1. 该不该move的三个问题：
- move之后，原对象还有意义吗？
合法，通常为空
- move会不会破坏不变式？
mutex的move很危险
- move后的对象，使用者能不能合理的继续使用？
接口层问题，如果move后，对象仍然能被调用，但行为看起来还行，其实是错的。 --> 设计失败

## 2. 哪些类型默认不该move
- 管理系统资源（socket/fd/mutex）
- 具有严格生命周期语义的对象
- 与外部世界强绑定的句柄
- move后的语义残缺对象

*适合move的类型*
- 纯资源容器（vector/string）
- 独占所有权对象(unique_ptr)
- move后为空仍然合理的类型

## 3.问题
如果你设计一个 FileLogger（内部持有一个打开的文件），你会让它：
A. 可 copy
B. 可 move
C. 都不允许
为什么？

C.
FileLogger它管理的是：
- 打开的文件描述符
- 文件偏移量（append 位置）
- 缓冲状态
- 可能的锁

不能被copy：因为存在多个资源占用的危险
不能被move: 语义成本太高，会引入模糊状态
一个logger被move之后，原logger还能被“合理使用”吗？