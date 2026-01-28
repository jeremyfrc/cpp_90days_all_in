## 背景（完全真实）

你现在有一个已经“工程正确”的版本：
```
class MetricsRegistry {
public:
    MetricsBuffer& create();
    MetricsBuffer& get(size_t i);
private:
    std::vector<std::unique_ptr<MetricsBuffer>> buffers_;
};
```

- 需求升级：
    - 多线程可能同时：
      - create()
      - get()
    - 在未来还可能 remove()
- MetricsBuffer 自身是线程安全的
- MetricsRegistry 现在不是

问题：在并发环境下，返回 MetricsBuffer& 是否安全？

## 任务

### 1 判断风险
请你回答（简要，但必须准确）：
1. 在当前设计下：
   - get() 返回引用，可能出现哪些并发问题？
   * 引用因 vector reallocation 而失效
   * 引用因未来 remove() 而悬空
   * 调用者无法知道引用的有效期
2. 哪些问题：
   - 来自 MetricsRegistry
   * 调用者无法知道引用的有效期
   * 生命周期管理
   * 并发结构修改 
   - 哪些不是 MetricsRegistry 的责任？
   * MetricsBuffer 内部数据竞争
   * buffer 自身的线程安全策略

- 答案及分析：
  - 风险来源
  1. vector reallocate → 引用失效
  2. 未来 remove() 的语义破坏
  3. Registry 的并发修改 ≠ Buffer 的并发访问
   
```
auto& buf = registry.get(0);
// 另一个线程
registry.create();  // vector 扩容

buf.push(1);  // ❌ buf 已经悬空
```
未定义行为(UB)

如果以后加如下：
```
void remove(size_t i);
```
那么get()返回的引用在任何时候都可以销毁

现在面对的问题是：
- 容器结构并发修改
- 生命周期失效
- 引用稳定性

### 2 写一个工程可接受的并发版本
你需要改造 MetricsRegistry，满足：
  - 支持多线程 create() / get()
  - 不破坏 Day 15 的 ownership 设计
  - 不返回悬空引用
  - 不暴露 mutex

你可以选择其中一种策略（任选其一）：
1. 方案 A（简单直接）
   - Registry 内部加锁
   - 明确约定：
   - 引用只在调用期间有效

2. 方案 B（工程常见）
   - 返回 stable handle（你自己设计）
   - 内部管理映射关系

## 标准答案的思考
*Day16先只给一个简版的code*
Handle 做了三件事：
- 把“使用权”与“生命周期”彻底分离
- 让 API 变得诚实：
  * 它不承诺“永远可用”
- 为 remove() 留下空间
👉 这是 Day 16 的灵魂

本期的代码解释：
  - handle-based API (Handle 是“访问凭证”，不是对象本身,资源本身都由registry负责，Handle管权限)
  - registry(管资源+并发，把“并发控制”和“业务代码”强制绑定在同一个作用域) + visitor
  - 生命周期与并发解耦
  - with管的是访问周期

## 现在这版设计，哪里还不安全？

*逻辑泄露*
当前handle被create()之后，并未被remove,或者remove了之后，仍在使用，这导致看起来合法，但是实际资源已经不存在了。
因为registry创建，用户持有，而这两者之间没有契约，handle可以长期存在，但随时可以失效，所以只是一种工程的折中。需要用一个valid flag来控制：
  - Handle ≠ 资源所有权
  - Handle 只是访问 token
  - 无效就 fail-fast
这在很多系统里都存在：
  - 文件描述符
  - GPU handle
  - OS object handle


## 函数引用返回必出Bug的点：
1. 悬空引用： 引用内容已被销毁，但是用户仍然要访问
2. 并发remove/clear， 两个线程一个access,一个remove
3. 容器扩容，reallocate
4. 锁生命周期断裂，因为返回之后，就不带锁了，如果并发读写，那必炸
5. 用户持有引用时间过长 --> 透支未来的控制权
当 Registry 内部资源可能被移除、移动、扩容或并发修改时，引用无法保证有效性和同步性，最终导致悬空引用或数据竞争。

handle + with在工程上叫做Scoped Access（作用域访问）。
在文件系统，GPU资源，数据库连接，并发容器这些场景都会用到这种设计模式。

## 练习

要求只有三条：
- create() 返回 Handle
- with(handle, fn) 内部加锁
- 用户永远拿不到引用存活到作用域外

代码骨架：
```
class SafeRegistry {
public:
    struct Handle { size_t id; };

    Handle create();

    template<typename F>
    void with(Handle h, F&& fn);

private:
    std::mutex m_;
    std::unordered_map<size_t, MetricsBuffer> data_;
    size_t next_id_ = 0;
};
```

你只需要补完：
- create()
- with()
- 不需要 remove，不需要 valid flag。

### Note:
1. try_emplace 的意义：
直接在 map 内部原地构造
避免临时对象
工程上这是惯用法。

2. fn 是用户提供的，可能非常慢，甚至可能死锁

Q 为什么这个SafeRegistry没有解决“锁内执行用户代码可能死锁”的问题
A Registry 无法控制 fn 会做什么

### 最典型死锁：
- 递归调用 with()
- 最典型死锁：递归调用 with()
- 锁顺序反转
锁内执行用户回调，是一种“强安全 + 弱可控”的设计
它避免悬空引用，但引入死锁与性能风险。

#### 所以真实系统会进一步演化出：
- 分层锁
- 更细粒度锁
- visitor 限制
- lock-free queue
- shared_ptr + epoch reclamation