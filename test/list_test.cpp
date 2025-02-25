/* 2025.02.23 */

#include "../header/thin_list.h"
#include <iostream>
#include <algorithm>
#include <list>

using namespace thinContainers;

class tstClass {
public:
    int m_data;
public:
    tstClass( int value ) : m_data( value ) {
        std::cout << m_data << " 构造函数\n";
    }
    ~tstClass() {
        std::cout << m_data << " 析构函数\n";
    }
};

void func( thin_list< int >::const_iterator iter ) {
    std::cout << *iter << std::endl;
}

void test01( void ) {
    thin_list< tstClass > lst;
    lst.emplace_back( 12 );
    lst.emplace_back( 19 );
    for ( auto elem : lst ) {
        std::cout << elem.m_data  << " ";
    }
    std::cout << std::endl;
    
}

int main( void ) {
    test01();
    return 0;
}