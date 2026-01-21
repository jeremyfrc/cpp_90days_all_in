## vector
```
std::vector<T> v;
v.push_back(t);
```

* 当v容量不够时会发生reallocation, vector 必须做 3 件事：*
- 分配一块 更大的新内存
- 把旧元素 搬到新内存
- 销毁旧内存中的元素

## STL 的真实决策逻辑
- if (T 的 move 构造是 noexcept) {
    用 move } 
- else if (T 可以 copy) {
    用 copy } 
- else {
    编译失败 }

要么操作成功，要么容器回到原样（Strong Guarantee）

## 三条工程直觉：
1. noexcept 不是性能优化，而是“异常安全声明”
2. STL 使用 move，是一种“奖励”，不是义务
3. 一个不能被 vector 安全管理的类型，工程价值极低

### examples
std::vector<std::mutex> 是非法的
而
std::vector<std::unique_ptr<std::mutex>>是合法的

* unique_ptr：
- move-only
- move 是 noexcept

* mutex：
- 固定在 heap 上
- 地址不变

## 函数定义总框架

- 只需要在两种情况下“显式定义/声明特殊成员函数”：
1. 改变默认语义
2. 阻止某些用法
如果既不改变，也不组织，那就什么都不写

## C++ 中，一个 class 有 6 类“特殊成员函数”
1. 默认构造
2. 析构
3. copy 构造
4. copy 赋值
5. move 构造
6. move 赋值
operator= 属于第 4 / 6 类，不是“随便的 operator”


## 何时定义这些成员函数
1. 纯值类型
不写构造；不写析构；不写copy/move；不写operator=

2. 要禁止某种行为
判断题（只要一个 YES 就该 delete copy）：
- 是否拥有独占资源？
    - 文件句柄 （存储资源等）
    - socket (TCP, Network等)
    - mutex
    - GPU context
- copy 后语义是否不明确？
- copy 是否可能导致 double free / 数据竞争？
如果是--> 必须delete copy

3. 要允许所有权转移
* 我希望这个对象能：*
- 从函数返回
- 放进 vector
- 被临时对象接管
👉 那就需要 move

4. 控制构造过程
* 需要：
- 打开文件
- 分配资源
- 校验参数
- 建立 invariant
构造函数 = 建立不变量（invariant）

## operator=什么时候必须显式出现
这个类型，是否允许赋值:
❌ 不允许 → operator=(...) = delete
✅ 允许且语义清晰 → = default
⚠️ 允许但逻辑特殊 → 自己写

## 什么时候需要private pointer
- 当你想把“对象的接口稳定性”与“实现细节的演化”彻底分离时
    - 这是公共库
    - 头文件会被大量 include
    - 实现可能频繁变化
    - 不想暴露 STL / mutex / 大型成员
- 你不希望：
    - 改一个成员
    - 触发整个项目 rebuild
- 或你在写：
    - SDK
    - 插件接口
    - 动态库

- 什么时候不该用private pointer
    - 小型 POD
    - 性能极度敏感的 tight loop
    - 内存布局必须连续（如 SIMD）

## 6个问题：
- 这是值，还是资源？
- copy后语义是否清晰？
- 是否允许多个实例指向同一资源？
- 是否需要转移所有权？
- 构造时是否要建立不变量？
- 是否需要隐藏实现、稳定 ABI？

## 工程判断口诀：
- 值类型：Rule of Zero
- 资源类型：RAII + delete copy + noexcept move
- 服务对象：pImpl + delete copy + noexcept move + 稳定接口