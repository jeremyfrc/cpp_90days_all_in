逻辑线程 + 渲染线程

逻辑线程（Game Thread）
更新世界状态
处理输入
物理模拟
AI
修改组件数据

渲染线程（Render Thread）
读取世界状态
提交 draw call
更新 GPU buffer
不应该修改游戏状态

Read-Copy-Update （RCU）：
读几乎无锁，写通过复制完成的模型，
核心思想：
读线程永远读“当前版本”
写线程创建一个新副本
切换指针
等所有读者结束
释放旧版本


优点：
读几乎无锁
非常适合读多写少

缺点：
内存翻倍
生命周期管理复杂
需要 epoch / hazard pointer 机制
在 Linux kernel 里非常常见。


Pipeline Concurrency
不是：
线程竞争同一资源
而是：
线程工作在不同阶段的数据版本

Temporal Isolation（时间隔离）

很多引擎：
Update Phase（单线程写）
Render Phase（只读快照）
End-of-frame commit

甚至更进一步：
Render 线程永远落后一帧
永远读取“上一帧完成状态”
这彻底消灭锁。

数据布局策略（data layout strategy）
SoA vs AoS
直接决定：
cache 命中率
SIMD 友好度
写放大程度
并发冲突概率

AoS（Array of Structures）
结构数据：
struct Entity {
    float x, y, z;
    float vx, vy, vz;
    int hp;
};

std::vector<Entity> entities;

内存布局：
[x y z vx vy vz hp][x y z vx vy vz hp][x y z vx vy vz hp]...

特点：
每个实体连续
适合“按对象访问”
cache line 包含多种字段

问题：
只更新 position 也会带上 velocity + hp
写操作污染无关字段的 cache

SoA（Structure of Arrays）
结构拆分：
struct World {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<int> hp;
};

内存布局：
x x x x x x x
y y y y y y y
z z z z z z z

特点：
同类数据连续
SIMD 非常友好
只写某个字段不会污染其他字段


如果逻辑线程只改 position：

在 AoS 中：
整个 Entity cache line 被改
render 读 hp 也会被 invalidation

在 SoA 中：
只 position vector 被写
hp vector cache 不受影响

写集中区：

在真实系统里，数据的写入往往不是均匀分布的。会有冷区和热区

工程做法：
Layer 1: Static world (只读) 不需要锁
Layer 2: Semi-static (低频写)  用分区锁
Layer 3: Dynamic hot (高频写)  用 SoA + 分块



正确顺序是：
数据规模
数据分布
访问模式
生命周期
最后才是锁策略
