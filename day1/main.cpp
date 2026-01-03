#include <iostream>
// 把 std::string 的声明 引入到当前编译单元中
// 让编译器在编译时认识 std::string 这个类型。
#include <string>
#include <unordered_map>

int main()
{
    std::unordered_map<std::string, std::string> kv;
    // 绝大多数存储在堆（heap）上，不是在栈上
    // kv 这个对象本身在 !栈上
    // 但是 buckets; nodes; key/value对象全部在 !堆上动态分配
    // [heap] bucket array   node -> key(string) -> value(string)

    kv["name"] = "Rich";
    // 等同于在kv中查找key: “name”, 如果不存在创建一个新的node，然后赋值
    //  [] operator会隐式创建元素
    kv["job"] = "Engineer";

    std::string key = "name";
    // key在这个地方创建，在main作用域结束后销毁

    if (kv.find(key) != kv.end()) // 这里用find而不用[]的原因如上
    {
        std::cout << "value = " << kv[key] << std::endl;
    }
    else
    {
        std::cout << "key not found" << std::endl;
    }

    return 0;
}
// return 0 之后，按顺序：
// key 析构
// kv 析构
// unordered_map析构： 释放堆上的nodes， 释放buckets，调用买个std::string的析构函数
// 栈帧被回收
// 程序结束，OS回收进程资源