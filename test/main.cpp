/* 2025.02.14
**测试标准库STL的容器所用
 */
#include <iostream>
#include <vector>

using namespace std;

int main( void )
{
    // vector< int > i_vec;
    int arr[8] = { 1, 2, 3, 4, 5 };
    std::cout << &arr[7] - &arr[0] << std::endl;
    return 0;
}