# A. 接口设计 (header first)
## 1. 这个接口在承诺什么？
头文件 = 契约，不是实现

## 2. 头文件里的每一个#include都合理吗？
如果删掉某个include，是否只影响实现，不影响使用？

# B. 所有权 & 资源管理
## 3. 这个类型是否直接管理资源
有没有写new/delete,或者持有raw pointer/file handle/mutex?
如果没有，rule of zero

## C. 值语义 vs 引用语义
## 4. 函数是否需要拥有这个参数？
只读：          const T&
修改但不拥有：   T&
获得所有权：     T(值语义)
不要引用表达所有权

# D. copy/move的工程判断
## 5. 默认选copy，只有在语义明确时选move
move的三个前提
- 所有权真的在转移
- 被move的对象不再被使用
- 接口有文档或命名暗示

# E. const是设计工具，不是修饰符
## 6. 哪些函数应该是const?
调用这个函数，会改变对象的逻辑状态吗？
会 -> 非const； 不会 -> const