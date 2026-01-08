#pragma once

#include <string>
#include <memory>

// 这里的Value并不拥有raw resource
// 资源被外包给了STL
struct Value
{
    std::string data;
};

class KVStore
// 直接拥有一个 raw pointer Impl_， 所以不能不考虑资源管理
{
public:
    KVStore();
    ~KVStore();

    void put(std::string key, Value value);

    bool get(const std::string &key, Value &out) const;

    bool contains(const std::string &key) const;

private:
    struct Impl;
    Impl *impl_;
    std::unique_ptr<Impl> impl1_;
};