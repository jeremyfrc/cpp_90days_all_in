# Task

## Context to complete code:
```
#include <vector>
#include <mutex>

class MetricsBuffer {
public:
    MetricsBuffer();

    void push(int v);
    int latest() const;

private:
    std::vector<int> data_;
    std::mutex m_;
};
```

## Scenario
你在一个服务端组件中实现一个 线程安全的指标缓冲区：
- 多个线程 push
- 少量线程 get
- 该类型 不会暴露给用户，是库内部类型
- 不考虑性能极致优化，只考虑 正确性 + 工程合理性


## 任务（核心）

补全 class 的 special member functions:
- 构造
- 析构
- copy / move 构造
- copy / move 赋值

明确写出：
- 哪些是 = delete
- 哪些是 = default
- 哪些你选择显式实现（如果有）

保证：
- 能通过编译
- 线程语义是自洽的

## 约束（非常重要）

- ❌ 不允许删除 std::mutex
- ❌ 不允许把 mutex 放进 shared_ptr
- ❌ 不允许“为了能 copy 而 copy”

因为 std::mutex：
❌ copy 不存在
❌ move 也不存在
编译器会 自动 delete move
所以不需要以下move构造函数：
```
MetricsBuffer(MetricsBuffer&&)
MetricsBuffer& operator=(MetricsBuffer&&)
```

总结
操作               |  状态	          | 原因
复制构造、复制赋值  |  = delete        |   std::mutex 不可复制
移动构造、移动赋值  |  = delete (自动)  |  std::mutex 不可移动
普通构造、析构      | = default        | 只需初始化/销毁成员
