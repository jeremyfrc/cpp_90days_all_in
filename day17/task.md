# remove(handle) 引入后，安全模型怎么升级？

## remove(handle) 会带来什么新风险
``` cpp
// 线程 A：
registry.with(h, [](MetricsBuffer& buf){
    buf.push(1);
});

// 线程 B 同时：
registry.remove(h);

// in remove:
data_.erase(h.id);
```
那么线程 A 拿到的 buf 会变成：
💥 悬空引用（Use-after-free）


## 工程最小解法：valid flag（不 erase）
```cpp
struct Entry {
    MetricsBuffer buffer;
    bool valid = true;
};

std::unordered_map<size_t, Entry> data_;
```

## Remove的正确语义
```cpp
void remove(Handle h) {
    std::lock_guard lock(m_);
    auto it = data_.find(h.id);
    if (it != data_.end()) {
        it->second.valid = false;
    }
}
```

## with() 必须升级：检查 valid
``` cpp
template<typename F>
void with(Handle h, F&& fn) {
    std::lock_guard lock(m_);

    auto it = data_.find(h.id);
    if (it == data_.end() || !it->second.valid) {
        throw std::out_of_range("Invalid handle");
    }

    fn(it->second.buffer);
}
```

## Task
```cpp
class SafeRegistry {
public:
    struct Handle { size_t id; };

    Handle create() {
        std::lock_guard lock(m_);
        size_t id = next_id_++;
        data_.emplace(id, Entry{MetricsBuffer{}, true});
        return {id};
    }

    void remove(Handle h) {
        // TODO
    }

    template<typename F>
    void with(Handle h, F&& fn) {
        // TODO
    }

private:
    struct Entry {
        MetricsBuffer buffer;
        bool valid;
    };

    std::mutex m_;
    std::unordered_map<size_t, Entry> data_;
    size_t next_id_ = 0;
};
```
基于骨架，补完TODO














