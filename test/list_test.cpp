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
    // thin_list< tstClass > lst = { 5, 4, 3, 99, };
    thin_list< int > lstt;// = { 8, 10, 12, 15 };
    thin_list< int > lst = { 1, 9, 10 };
    lstt.merge( lst );
    for ( auto elem : lstt ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << "size = " << lstt.size() << std::endl;
 
    
}

int main( void ) {
    test01();
    return 0;
}