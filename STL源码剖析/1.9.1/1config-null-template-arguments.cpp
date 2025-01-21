/* 2025.01.21
** __STL_NULL_TMPL_ARGS
 */
#include <iostream>

using namespace std;

class alloc {};

template < typename T, typename Alloc = alloc, size_t BufSiz = 0 >
class deque {
public:
    deque()
    {
        cout << "deque" << ' ';
    }
};

#if 0
template < typename T, typename Sequence >
class stack;

template < typename T, typename Sequence >
bool operator==( const stack< T, Sequence >& x, const stack< T, Sequence >& y );

template < typename T, typename Sequence >
bool operator<( const stack< T, Sequence >& x, const stack< T, Sequence >& y );

#endif

template < typename T, typename Sequence = deque< T > >
class stack {
    // friend bool operator== <T> ( const stack< T >&, const stack< T >& );
    // friend bool operator< <T> ( const stack< T >&, const stack< T >& );
template < typename U, typename SS >
    friend bool operator==( const stack< U, SS >&, const stack< U, SS >& );
template < typename U, typename SS >    
    friend bool operator<( const stack< U, SS >&, const stack< U, SS >& );
public:
    stack() { cout << "stack" << endl; }
private:
    Sequence c;
};

template < typename T, typename  U  >
bool operator==( const stack< T, U >& x, const stack< T, U >& y )
{
    cout << "operator==" << '\t';
    return 1;
}

template < typename T, typename Sequence   >
bool operator<( const stack< T, Sequence >& x, const stack< T, Sequence >& y )
{
    cout << "operator<" << '\t';
    return 1;
}

int main( void )
{
    stack< int > x, y;
    stack < char > c;
    // cout << ( x == c) << endl;
    cout << ( x < y ) << endl;
    return 0;
}
