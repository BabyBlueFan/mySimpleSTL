#include <type_traits>
#include <iostream>

// 仅对整数类型启用此函数
// template <typename T>
// typename std::enable_if<std::is_integral<T>::value>::type
template < typename T >
void print(T value, typename std::enable_if< std::is_integral< T >::value >::type* = nullptr ) {
    std::cout << "Integral: " << value << std::endl;
}

// 仅对浮点类型启用此函数
template < typename T, typename = typename std::enable_if< std::is_floating_point< T >::value >::type >
// typename std::enable_if<std::is_floating_point<T>::value>::type
void print(T value) {
    std::cout << "Floating: " << value << std::endl;
}

template < typename T >
void func() {
    std::cout << "xxx" << std::endl;
}

int main() {
    print(42);      // 调用整数版本
    print(3.14);    // 调用浮点版本
    // print("hello"); // 编译错误：无匹配函数
}