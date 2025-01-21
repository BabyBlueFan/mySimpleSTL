/* 2025.01.21
** test：仿函数用于 for_each()
 */

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template < typename T >
class print {
public:
    void operator()( const T& elem ) { cout << elem << ' '; }
};

int main( void )
{
    int ia[6] = { 0, 1, 2, 3, 4, 5, };
    vector< int > iv( ia, ia +6 );
    //print< int >() 是一个临时对象，不是一个函数调用
    for_each( iv.begin(), iv.end(), print< int >() );
    return 0;
}
