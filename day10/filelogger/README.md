## A file logger

# 文件句柄是“独占资源（unique ownership）”; copy 会制造 两个对象声称自己拥有同一资源; move 则是 明确的所有权转移

# moved-from 的 FileLogger 处于“有效但未指定状态（valid but unspecified）”

# 如果一个类型的 move 可能抛异常，STL 容器在关键场景下会退回使用 copy。
# move 必须是 noexcept，否则这个类型在容器中是“半残废”的。
假设：
```
std::vector<FileLogger> logs;
logs.push_back(FileLogger("a.log"));
logs.push_back(FileLogger("b.log"));
```

当 vector 扩容时：
它必须把旧元素搬到新内存
它会问一个问题：
“我能不能保证 move 不会抛异常？”

如果 move 是 noexcept:
✅ 使用 move
✅ 高效
✅ 安全

# 因为 FileLogger 不直接管理任何底层资源，它的所有资源都由成员对象（std::ofstream）管理，而 ofstream 本身是 RAII 类型。

# noexcept = default: “销毁 FileLogger 绝不会抛异常。”
