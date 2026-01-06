#include <iostream>
#include <string>
#include <unordered_map>

struct Value
{
    std::string data;

    Value() : data("")
    {
        std::cout << "Value default-constructed\n";
    }

    Value(const std::string &d) : data(d)
    {
        std::cout << "Value constructed from string\n";
    }

    Value(const Value &other) : data(other.data)
    {
        std::cout << "Value copy-constructed\n";
    }

    Value &operator=(const Value &other)
    {
        std::cout << "Value copy-assigned\n";
        data = other.data;
        return *this;
    }

    Value(Value &&other) noexcept : data(std::move(other.data))
    {
        std::cout << "Value move-constructed\n";
    }

    Value &operator=(Value &&other) noexcept
    {
        std::cout << "Value move-assigned\n";
        data = std::move(other.data);
        return *this;
    }

    ~Value()
    {
        std::cout << "Value destructed\n"
                  << std::endl;
    }
};

class KVStore
{
public:
    void put(std::string key, Value value)
    {
        // kv_[key] = value;
        // kv_[std::move(key)] = std::move(value);
        kv_.emplace(std::move(key), std::move(value));
    }

    bool get(const std::string &key, Value &out)
    {
        auto it = kv_.find(key);
        if (it == kv_.end())
        {
            return false;
        }
        out = it->second;
        return true;
    }

private:
    std::unordered_map<std::string, Value> kv_;
};

int main()
{
    KVStore store;

    Value v("Hello");
    store.put("a", v);

    Value out("init");
    store.get("a", out);

    return 1;
}

// 现代 C++ KVStore 对象生命周期图
/*
┌─────────────────────────────┐
│ Value v("Hello")            │
│ constructed from string     │
└───────────────┬─────────────┘
                │
                ▼
      ┌───────────────────────┐
      │ store.put("a", v)     │
      │ kv_[key] = std::move(value)
      └───────────┬───────────┘
                  │
                  ▼
      ┌────────────────────────────┐
      │ Map internal Value object  │
      │ default-constructed        │
      └───────────┬────────────────┘
                  │
                  ▼
      ┌────────────────────────────┐
      │ move-assigned from v       │
      └────────────────────────────┘
                  │
                  ▼
┌───────────────────────────────────────────────┐
│ Value out("init")                             │
│ constructed from string                       │
└─────────────────────────────┬─────────────────┘
                              │
                              ▼
                  ┌───────────────────────┐
                  │ out = it->second      │
                  │ move-assigned         │
                  └───────────────────────┘

───────────────────────────────────────────────
│ 析构顺序（逆序）                              │
│ 1. out                                       │
│ 2. v                                         │
│ 3. Map internal Value                        │
│ 4. Map internal string key                   │
───────────────────────────────────────────────
*/
// KVStore 对象生命周期时间轴
/*
Time ──────────────────────────────►

1. Value v("Hello") created
   ┌─────────────────────────────┐
   │ Value v                     │
   │ constructed from string     │
   └─────────────────────────────┘

2. store.put("a", v) begins
   ┌─────────────────────────────┐
   │ Map internal Value          │
   │ default-constructed         │
   └─────────────────────────────┘
           │
           ▼
   Move-assignment from v
   ┌─────────────────────────────┐
   │ Map internal Value          │
   │ move-assigned               │
   └─────────────────────────────┘

3. Value out("init") created
   ┌─────────────────────────────┐
   │ Value out                   │
   │ constructed from string     │
   └─────────────────────────────┘

4. store.get("a", out)
           │
           ▼
   Move-assignment to out
   ┌─────────────────────────────┐
   │ Value out                   │
   │ move-assigned               │
   └─────────────────────────────┘

───────────────────────────────
Destruction (reverse order of stack + container):

5. Value out destroyed
   ┌─────────────────────────────┐
   │ Value out                   │
   │ destructed                  │
   └─────────────────────────────┘

6. Value v destroyed
   ┌─────────────────────────────┐
   │ Value v                     │
   │ destructed                  │
   └─────────────────────────────┘

7. Map internal Value destroyed
   ┌─────────────────────────────┐
   │ Map internal Value          │
   │ destructed                  │
   └─────────────────────────────┘

8. Map internal string key destroyed
   ┌─────────────────────────────┐
   │ Map key "a"                 │
   │ destructed                  │
   └─────────────────────────────┘
*/