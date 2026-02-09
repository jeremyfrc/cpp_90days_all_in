# Day19
把你现在的 SafeRegistry，推到“真实生产系统能用”的边界

## 当前版本在工程上不够安全
### 用户代码 = 不可信代码
为什么 fn(it->second.buffer) 是炸弹？
- 你无法控制
- 可能：
  - 再调用 registry.with(...) → 自锁死锁
  - 调用 remove(h) → 修改同一结构
  - 执行耗时 IO / sleep → 锁被长期占用
  - 抛异常 → 锁释放，但状态可能不一致

### 你到底要保护什么？
- 你现在用一个 mutex 保护了一切：
  - handle 查找
  - 生命周期
  - buffer 的访问
- 但在真实系统里会遇到：
  - 性能问题
    - 所有 with() 串行
    - 即使访问不同 MetricsBuffer
  - 语义混乱
    - Registry 的锁
    - Buffer 内部自己的锁

### “安全” vs “可用”
避免了悬垂引用，容器扩用失效，use-after-free
代价是API复杂，性能瓶颈，用户写起来憋屈
*权衡*

## 进阶方案
### 设计原则：
  - Registry：
    - 只负责：创建 / 查找 / remove
  - MetricsBuffer：
    - 自己保证线程安全
  - with：
    - 不在锁内执行用户代码

新版本解决了引用失效，容器rehash， use-after-free，锁内执行用户代码，fn重入registry的问题，但是接受了shared_ptr引用计数成本，remove后buffer可能仍存活一小段时间，生命周期延迟释放的代价。

### 和下面系统是同一类设计：
- Linux fd + refcount
- Java object handle
- Vulkan handle
- Chrome Mojo
- 大量监控 / metrics 系统

## 思考题
1. 什么时候不用 shared_ptr，而用 generation + unique_ptr？
不允许 资源在“用户代码执行期间”被延长生命周期时

2. MetricsBuffer 里的 mutex 是否还需要？为什么？
需要，而且职责不同。

3. 如果 fn 很慢，shared_ptr 会带来什么副作用？
生命周期被延长，资源压力放大，顺序语义被破坏(remove/shutdown的语义被弱化)

### 典型使用 generation + unique_ptr 的场景
1. OS / 内核级资源
- file descriptor
- epoll handle
- GPU context
- socket
2. 资源释放 = 语义事件
- close() 之后 必须立刻失效
- 不允许“还有人拿着旧对象在跑”
3. 安全 / 正确性 > 易用性
- shared_ptr 的“延迟释放”是不可接受的

