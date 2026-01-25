## 工程背景

MetricsBuffer: 
- ❌ 不可 copy
- ❌ 不可 move
- ✅ 线程安全
- ✅ 内部状态型对象

但是现在来了一个新的工程需求：
一个模块需要“管理多个 MetricsBuffer 实例”

## 约束
- 不能：
    - ❌ 给 MetricsBuffer 加 copy
    - ❌ 给 MetricsBuffer 加 move
    - ❌ 改 MetricsBuffer 的 public API
- 可以：
    - ✅ 改“外层设计”
    - ✅ 用 STL
    - ✅ 引入中间层类型

## 任务
设计一个新的 class：MetricsRegistry 并满足：
- 能管理 多个 MetricsBuffer
- 内部不发生 copy / move MetricsBuffer
- 使用方式 工程上合理
- 代码能编译


## Lessons:
“不能 move” ≠ “不能用”
indirection, ownership, address stability
std::unique_ptr 在类型系统中表达“唯一所有权”
