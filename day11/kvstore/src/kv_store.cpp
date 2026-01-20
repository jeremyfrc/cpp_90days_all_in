#include "kv_store.h"
#include <unordered_map>

struct KVStore::Impl
{
    std::unordered_map<std::string, Value> map;
};

KVStore::KVStore() : impl_(std::make_unique<Impl>())
{
}

KVStore::~KVStore() = default;

void KVStore::put(std::string key, Value value)
{
    impl_->map.emplace(std::move(key), std::move(value));
}

bool KVStore::get(const std::string &key, Value &out) const
{
    auto it = impl_->map.find(key);
    if (it == impl_->map.end())
    {
        return false;
    }
    out = it->second;
    return true;
}

bool KVStore::contains(const std::string &key) const
{
    return impl_->map.find(key) != impl_->map.end();
}