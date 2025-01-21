#include <iostream>

// 类模板 MyTemplateClass
template <typename T, typename S = double >
class MyTemplateClass {
private:
    T privateData;

public:
    MyTemplateClass(T data) : privateData(data) {}

    // 声明模板友元函数
    template <typename U, typename P>
    friend void printData(MyTemplateClass<U, P>& obj);
};

// 定义模板友元函数
template <typename T, typename U>
void printData(MyTemplateClass<T, U>& obj) {
    std::cout << "Private data: " << obj.privateData << std::endl;
}

int main() {
    MyTemplateClass<int> intObj(42);
    printData(intObj);  // 调用模板友元函数

    MyTemplateClass<double> doubleObj(3.14);
    printData(doubleObj);  // 同样可以调用模板友元函数

    return 0;
}