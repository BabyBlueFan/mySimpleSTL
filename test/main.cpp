#include <iostream>

template < typename T >
class myClass {
public:
    T m_a;
    T m_b;
    myClass( T a = T(), T b = T() ) : m_a( a ), m_b( b ) {}
    friend bool operator==( const myClass<T>& x, const myClass<T>&  y){
        return x.m_a == x.m_b;
    }
};

/* template < typename T >
bool operator==( const myClass< T >& x, const myClass< T >& y ) {
    return x.m_a == x.m_b;
} */

int main( void ) {
    myClass< int > a, b;
    myClass( 10, 17 ) == b;
    myClass< std::string > c;
    // a == c;
    return 0;
}
