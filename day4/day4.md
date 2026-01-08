# 3种函数接口：
## 1. 值传递（Value semantics）
```
void put(Value value);
```
调用者：
- lvalue → copy
- rvalue → move
函数内部：
- 明确拥有这个对象
- 可以随意 move

## 2. const引用（只读）
```
void put(const Value& value);
```
- 不拥有
- 不修改
- 不负责生命周期

## 3. 右值引用（高级、危险）
```
void put(Value&& value);
```
- 强制调用者放弃对象
- 接口侵入性强
- 容易误用

所有权清晰 → 设计可扩展 → 性能自然好

## Rule of Zero: 
** 如果你的类只管理“值类型成员”，那你最好一个特殊成员函数都不要写。**
```
struct Value {
    std::string data;
};
```
没有其他的成员定义

## 必须写Rule of Five
管理资源而不是值
见file.cpp
因为你在管理“唯一资源”
Rule of Zero = 稳健路径
Rule of Five = 明确需要时的下注
