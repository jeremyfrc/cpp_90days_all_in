// 头文件的真正职责不是“能不能编译”而是定义使用者需要知道什么
// 任何放进头文件的东西都会影响 所有包含它的编译单元，成为公共依赖，
// 固化为ABI/API的一部分，具体实现的时候，更有弹性。
// 实现细节不进头文件，是为了降低耦合、稳定接口、保护未来选择权。

#include "kv_store.h"
#include <iostream>

int main()
{
    KVStore store;

    store.put("a", Value{"hello"});

    Value v;
    if (store.get("a", v))
    {
        std::cout << v.data << std::endl;
    }
}