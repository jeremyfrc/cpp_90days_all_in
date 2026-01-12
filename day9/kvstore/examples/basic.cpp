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
    return 0;
}