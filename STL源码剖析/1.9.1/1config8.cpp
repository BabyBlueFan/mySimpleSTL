/* 2025.01.21
**__STL_MEMBER_TEMPLATES -- 类模板内定义函数模板
 */

#include <iostream>
using namespace std;

class alloc {};
template < typename T, typename Alloc = alloc >
class vector {
public:
    typedef T value_type;
    typedef value_type* iterator;

    template < typename I >
    void insert( iterator position, I first, I lase ) {
        cout << "insert()" << endl;
        }
    };

    int main( void ) {
        int ia[5] = { 0, 1, 2, 3, 4, };

        vector< int > x;
        vector< int >::iterator ite;
        x.insert( ite, ia, ia +5 );
    };