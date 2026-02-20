#include <iostream>
#include <cmath>

class Vector3D {
public:
    double x, y, z;

    // 1. 使用统一初始化列表 (Member Initializer List)
    // 这是最高效的初始化方式，直接在内存中构建，避免先默认构造再赋值。
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0) 
        : x{x}, y{y}, z{z} {}

    // 2. 运算符重载：实现 v1 + v2
    // 注意：使用 const Vector3D& 避免拷贝，返回新对象。
    Vector3D operator+(const Vector3D& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    // 3. 这里的 const 保证了我们可以计算只读对象的模长
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};