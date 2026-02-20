# 新的训练框架 Delivery-Driven C++ Plan
每天固定结构
每一天只做一题，但必须完整走完工程流程：

## SPEC 

### 工程需求说明
必须先输出：
- API 设计
- 数据结构选择
- 复杂度分析
- 失败模式分析
👉 没设计就写代码 = 判定为失败

### Coding 阶段（必须满足 4 条）
代码必须：
- 可编译
- 可运行
- 无 UB
- 无数据竞争

## 任务：实现一个线程安全 Metrics Aggregator
### 支持多线程同时：
```cpp
record(key, value)
```
### 并支持另一个线程：
```cpp
snapshot()
```
### 返回当前所有 key 的统计值：
```cpp
count
sum
avg
max
min
```
### 限制条件（真实工程约束）
- record QPS ≥ 5M/s（模拟测试即可）
- snapshot 不能阻塞 record > 1ms
- key 为 string
- key 数量 ≤ 1e6
- 内存 ≤ 512MB

### 必须实现 API
```cpp
class Metrics {
public:
    void record(std::string_view key, double v);

    struct Stat {
        uint64_t count;
        double sum;
        double min;
        double max;
    };

    std::unordered_map<std::string, Stat> snapshot() const;
};
```








