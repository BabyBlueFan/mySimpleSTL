/* 2025.01.21
**
 */
#include <iostream>
using namespace std;

class alloc {};
template < typename T, typename Alloc = alloc >
class vector {
public:
    void swap( vector< T, Alloc >& ) 
    {
        cout << "swap()" << endl;
    }
};


int main( void )
{   
    vector< int > x, y;
    swap( x, y );
    return 0;
}

