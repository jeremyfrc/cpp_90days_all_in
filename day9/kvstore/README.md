# KVStore — 一个小而稳的 C++ 键值存储组件

## 定位
** 实用型（Production-minded）：**
- 接口稳定
- 行为可预测
- 误用成本低
- 设计优先于技巧

本库不追求功能多，而追求：敢被别人用、敢被维护。

## 设计原则（TL;DR）
1. 接口即契约：头文件只暴露必要信息
2. 值语义优先：通过类型系统表达所有权
3. RAII 管理资源：避免手写 new/delete
4. 默认安全：copy / move 行为由设计自然决定
5. 实现可替换：不锁死内部数据结构

## 目录结构
```
kvstore/
├── include/
│ └── kv_store.h
├── src/
│ └── kv_store.cpp
├── examples/
│ └── basic.cpp
└── README.md
```

如何 build
如何 run
C++ 标准要求

```
mkdir build
cd build
cmake ..
cmake .. -G "MinGW Makefiles"
cmake --build .
build\Debug\kvstore_example.exe
./example.exe
```

## build 下的文件介绍:

在 MSVC 多配置生成器中：
Debug / Release 是并行存在的
CMake 不知道你“当前想跑哪个”
所以它会生成：
```
build/
├── Debug/
│   └── kvstore_example.exe
├── Release/
│   └── kvstore_example.exe
├── kvstore.sln
├── example.vcxproj （日志文件）
└── ...
```
