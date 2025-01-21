/* 2025.01.21
**仿函数
 */

#include <iostream>
//using namespace std;

//由于将operator()重载了，因此plus成了一个仿函数
template < typename T >
struct plus {
    T operator()( const T& x, const T& y ) const 
    {
        return x + y;
    }
};

//由于将operator()重载了，因此minus成了一个仿函数
template < typename T >
struct minus {
    T operator()( const T& x, const T& y ) const
    {
        return x - y;
    }
};

int main( void )
{
    plus< int > plusobj;
    minus< int > minusobj;
    std::cout << plusobj( 3, 5 ) << std::endl;
    std::cout << minusobj( 3, 5 ) << std::endl;
    //以下是直接产生仿函数的临时对象
    std::cout << plus< int >()( 43, 50 ) << std::endl;
    std::cout << minus< int >()( 43, 50 ) << std::endl;
    return 0;
}