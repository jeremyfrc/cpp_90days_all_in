# Day20｜并发 API 设计 Checklist

## 一、对象暴露方式（第一性问题）

❌ API 返回：
- 引用（T&）
- 指针（T*）
你必须问：
1. 生命周期谁负责？
2. 并发期间会不会失效？
3. 容器 rehash / resize 会不会炸？

✅ 推荐返回：
- Handle / Token
- shared_ptr（有成本）
- 值语义对象（最安全

## 二、生命周期模型

- 模型 A：RAII / 独占
  - unique_ptr
  - generation + handle
  - 明确 create / destroy
  ✔ 可控
  ❌ 不灵活

- 模型 B：共享生命周期
  - shared_ptr
  - weak_ptr
  ✔ 易用
  ❌ 延迟析构 / 资源峰值不可控

- 模型 C：外部管理（危险）
  - raw pointer
  - 引用
  ❌ 默认不选，除非你在写 STL

## 三、锁的边界（这是并发 API 的灵魂）

❌ 错误信号：
- “用户自己保证线程安全”
- “调用者别在 fn 里干坏事”

✅ 正确问题：
1. 锁是在 API 内，还是 API 外？
2. fn 是在锁内执行，还是锁外？
3. 锁是否可重入？
4. 是否可能用户在 fn 中再调用 registry？

经验法则：
锁内：短、确定、不调用用户代码
锁外：长、不确定、用户逻辑

## 一个你可以直接用的“启动流程”

Step1 用中文写 5 行
  * 这个类拥有什么？
  * 这个类不允许什么？
  * 并发下必须保证什么？
  * 生命周期由谁控制？
  * 用户最容易怎么用错？

Step2 先写 class 壳子（不写实现）
```cpp
class X {
public:
    X();
    X(const X&) = delete;
    X& operator=(const X&) = delete;

    Handle create();
    void remove(Handle);
    template<typename F>
    void with(Handle, F&&);
};
```

Step3 再决定
    -- 是否需要 mutex
    -- 是否需要 shared_ptr
    -- 是否需要 generation

Step4 最后才写实现
