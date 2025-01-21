/* 2025.01.21
** __STL_LIMITED_DEFAULT_TEMPLATES -- 模板参数可否根据前一个模板参数设定默认值
 */

#include <iostream>

using namespace std;

class alloc {};
template < typename T, typename Alloc = alloc, size_t BufSiz = 0 >
class deque {
public:
    deque()
    {
        cout << "deque()" << endl;
    }
};

//根据前一个参数值T,设定下一个参数Sequence的默认值为deque<T>
template < typename T, typename Sequence = deque< T > >
class stack {
public:
    stack() 
    {
        cout << "stack" << endl;
    }
private:
    Sequence c;
};

int main( void )
{
    stack< int > x;
    return 0;
}