## KVStore 不能 copy，是一个“设计选择”，不是语言限制。

## Value 是：
- 纯值语义（string）
- 没有身份
- 没有共享状态
## Copy 一个 Value：
- 语义明确
- 成本可控
- 行为可预测

## 因为它们的资源成员本身就是 move-safe 的，并且 move 后源对象仍然是“可析构、可销毁”的合法状态。
因为它们的资源成员本身就是 move-safe 的，并且 move 后源对象仍然是“可析构、可销毁”的合法状态。

## 加了 Mutex 之后
std::mutex 本身不可 copy、不可 move

工程上的常见解决方式是：
- mutex 留在 Impl 内
- KVStore 自身仍然 move（转移整个 Impl）
- 不暴露 mutex 的移动语义

## pImpl + move + noexcept 解决了什么工程问题？
### 1. pImpl 解决的是 什么问题
    - 头文件稳定（ABI / 编译依赖）
    - 隐藏实现细节
    - 降低 rebuild 成本
    - 私有成员可随时变

### 2. move 解决的是 对象生命周期问题
KVStore 可以：
- 从函数返回
- 放进容器
- 作为成员被转移

而不是：
- 被强制 heap 分配
- 被强制用指针管理

### 3. noexcept 解决的是 STL 信任问题
std::vector<KVStore> 扩容时：
- 如果 move 是 noexcept → 用 move
- 否则 → 尝试 copy（而你又 delete 了）
👉 没有 noexcept = 类型在 STL 中退化为二等公民


## 我如何设计一个：
- 对用户稳定
- 对实现自由
- 对 STL 友好
- 对性能不妥协
的公共库类型