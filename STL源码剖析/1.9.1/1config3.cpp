/*2025.01.21
**__STL_STATIC_TEMPLATE_MEMBER_BUG -- 类模板中有静态数据成员 
 */
#include <iostream>

template < typename T >
class testClass {
public:
    static int _data;
};

//初始化静态数据成员
template < typename T >
int testClass< T >::_data = 9;

template < >
int testClass< char >::_data = 66;

int main( void )
{
    testClass< int > int_obj;
    testClass< char > char_obj;
    std::cout << int_obj._data << std::endl;
    std::cout << char_obj._data << std::endl;
    testClass< char > char_obj1;
    char_obj1._data = 100;
    std::cout << int_obj._data << std::endl;
    std::cout << char_obj._data << std::endl;
    return 0;
}