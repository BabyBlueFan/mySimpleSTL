/* 2025.02.23 */

#include "../header/thin_list.h"
#include <iostream>
#include <algorithm>
#include <list>

using namespace thinContainers;

class tstClass {
private:
    int m_data;
public:
    tstClass( int value ) : m_data( value ) {
        std::cout << m_data << " 构造函数\n";
    }
    ~tstClass() {
        std::cout << m_data << " 析构函数\n";
    }
};

void test01( void ) {
    thin_list< int > lst{ 4, 5, 7, 99, 21, 90 };
    thin_list< int > lstt( lst.begin(), lst.end() );
    lstt.back() = 15;
    lstt.front() = 15;
    // lstt.front() = 88;
    /* lst.push_back( 8 );
    lst.push_back( 9 ); 
    lst.push_back( 10 ); 
    lst.push_back( 3 ); 
    lst.push_back( 2 ); */
    for ( auto elem : lstt ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl << "size = " << lstt.size() << std::endl;
    // thin_list< int >::const_iterator iter = lst.begin();
    return;
}

int main( void ) {
    test01();
    return 0;
}