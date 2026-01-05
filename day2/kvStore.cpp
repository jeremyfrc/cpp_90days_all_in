#include <iostream>
#include <string>
#include <unordered_map>

class KVStore
{
public:
    KVStore()
    {
        std::cout << "KVStore constructed " << std::endl;
    }

    ~KVStore()
    {
        std::cout << "KVStore destructed" << std::endl;
    }

    void put(const std::string &key, const std::string &value)
    {
        kv_[key] = value;
    }

    bool get(const std::string &key, std::string &out)
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
    std::unordered_map<std::string, std::string> kv_;
};

int main()
{
    std::cout << "Enter main \n";

    {
        KVStore store;
        store.put("name", "Rich");

        std::string value;
        if (store.get("name", value))
        {
            std::cout << "value = " << value << std::endl;
        }
    }

    std::cout << "Exit main \n";
    return 1;
}

// RAII understanding
// will print out as below:
/*
Enter main
KVStore constructed
value = Rich
KVStore destructed
Exit main
*/

// This is to understand 对象的生命周期 = 作用域，而不是程序结束
// 你没有写一行 delete，但你完全控制了资源释放。
// what I learned today:
// 1.对象的生命周期由作用域决定
// 2.析构函数在离开作用域时自动调用
// 3.我没有动手管理内存，但资源被正确释放