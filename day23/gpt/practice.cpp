//1 practice to write a class
#include <string>
#include <unordered_map>

class CounterMap{
public:
    CounterMap() = default;
    void inc(const std::string& key);
    bool get(const std::string& key, int& value) const;
    std::unordered_map<std::string, int> snapshot() const;

private:
    std::unordered_map<std::string, int> map_;
};


//2 practice to implement inc

void CounterMap::inc(const std::string& key){
    auto it = map_.find(key);
    if (it == map_.end()){
        map_[key] = 1;
    } else {
        it->second++;
    }


    auto& value = map_.try_emplace(key, 0).first->second;
    value++;
};

//3. practice to implement get
bool CounterMap::get(const std::string& key, int& value) const {
    auto it = map_.find(key);
    if (it == map_.end()){
        return false;
    } else {
        value = it->second;
        return true;
    }
}


// 4. practice to implement snapshot
std::unordered_map<std::string, int> CounterMap::snapshot() const {
    std::unordered_map<std::string, int> snap;
    for (const auto& [k, v] : map_){
        snap[k] = v;
    }
    return snap;
};

// 5. add mutex to CounterMap
#include <mutex>

class CounterMapV2{
public:
    CounterMapV2() = default;
    void inc(const std::string& key);
    bool get(const std::string& key, int& value) const;
    std::unordered_map<std::string, int> snapshot() const;
private:
    std::unordered_map<std::string, int> map_;
    std::mutex m_;
};

void CounterMapV2::inc(const std::string& key){
    std::lock_guard<std::mutex> lock(m_);
    auto& value = map_.try_emplace(key, 0).first->second;
    value++;
}

bool CounterMapV2::get(const std::string& key, int& value) const{
    std::lock_guard<std::mutex> lock(m_);
    auto it = map_.find(key);
    if (it == map_.end()){
        return false;
    } else {
        value = it->second;
        return true;
    }
}

std::unordered_map<std::string, int> CounterMapV2::snapshot() const {
    std::lock_guard<std::mutex> lock(m_);
    return map_;
}


// 6. 性能优化练习：
// 改成 shared_mutex：读不阻塞读
// 分段锁：map 分多段，每段独立锁
// lock-free 或 atomic 实现（进阶）

#include <shared_mutex>

class CounterMapV3{
public:
    CounterMapV3() = default;
    void inc(const std::string& key);
    bool get(const std::string& key, int& out) const;
    std::unordered_map<std::string, int> snapshot() const;
private:
    std::unordered_map<std::string, int> map_;
    mutable std::shared_mutex rwLock_;
};

void CounterMapV3::inc(const std::string& key){
    std::unique_lock<std::shared_mutex> lock(rwLock_);
    auto& value = map_.try_emplace(key, 0).first->second;
    value++;
}

bool CounterMapV3::get(const std::string& key, int& out) const {
    std::shared_lock<std::shared_mutex> lock(rwLock_);
    auto it = map_.find(key);
    if (it == map_.end()){
        return false;
    } else{
        out = it->second;
        return true;
    }
}

std::unordered_map<std::string, int> CounterMapV3::snapshot() const {
    std::shared_lock<std::shared_mutex> lock(rwLock_);
    return map_;
}



