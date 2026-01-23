# Adding mutex + pImpl

## 结论
mutex只能放在Impl里，KVStore本身仍然是move-only,noexcept的服务对象

## 正确的方法： mutex只能进Impl
*正确的结构：*
```
// kv_store.cpp
struct KVStore::Impl{
    std::mutex m;
    std::unordered_map<std::string, Value> kv;
};
```

## put/get的正确加锁位置
*put*   cpp
```
void KVStore::put(std::string key, Value value){
    std::lock_guard<std::mutex> lock(impl_->m);
    impl_->kv.emplace(std::move(key), std::move(value));
}
```

*get*   cpp
```
bool KVStore::get(const std::string& key, Value& out) const{
    std::lock_guard<std::mutex> lock(impl_->m);
    auto it = impl_->kv.find(key);
    if (it == impl_->kv.end()){
        return false;
    }
    out = it->second;
    return true;
}
```
mutex固定在heap上，KVStore只是换了owner

## 隐含不变量
KVStore 的所有 public 方法，都必须通过 Impl 访问共享状态。
以后加：
- cache
- stats
- condition_var
- rwlock
都不需要改头文件。

const 只约束“逻辑语义”，不约束并发可见性。如果 get() 不加锁，在并发 put() 时会产生 data race
线程安全不是“加锁”，而是“在不破坏类型语义的前提下控制共享状态”。
