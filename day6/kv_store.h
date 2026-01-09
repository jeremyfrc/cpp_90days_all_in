#pragma once
#include <string>
#include <memory>

struct Value
{
    std::string data;
};

class KVStore
{
public:
    KVStore();
    void put(std::string key, Value value);
    bool get(const std::string &key, Value &out) const;
    bool contains(const std::string &key) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
