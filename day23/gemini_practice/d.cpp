#include <iostream>

template <typename T>
class Vector3D {
public:
    T x, y, z;

    // 使用我们学过的 {} 初始化
    Vector3D(T x = {}, T y = {}, T z = {}) 
        : x{x}, y{y}, z{z} {}

    // 进阶优化：复合赋值运算符 (+=) 
    // 这种写法通常比直接重载 + 更快，因为它不产生临时副本。
    Vector3D& operator+=(const Vector3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this; 
    }

    // 利用 += 实现 + (这是 C++ 开发中的标准惯用法)
    friend Vector3D operator+(Vector3D lhs, const Vector3D& rhs) {
        lhs += rhs; // 注意 lhs 是按值传递的，这里利用了编译器的拷贝优化
        return lhs;
    }

    constexpr T dot(const Vector3D<T>& other) const{
        return x*other.x + y*other.y + z*other.z;
    }
};

// 实际使用
int main() {
    // 编译器会自动推导类型吗？C++17 以后可以！
    Vector3D<double> v1{1.0, 2.0, 3.0};
    Vector3D<double> v2{4.0, 5.0, 6.0};

    auto v3 = v1 + v2; // 极其自然的数学语法
    
    std::cout << "Result: " << v3.x << ", " << v3.y << ", " << v3.z << std::endl;
    return 0;
}


template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}
// 用decltype追踪返回类型 (Trailing Return Type)
// C++14 以后可以用auto来直接推导返回类型


