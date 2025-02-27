/* 2025.02.23 */

#include "../header/thin_list.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <functional>

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

bool myComp( int a, int b ) {
    return a > b;
}

void test01( void ) {
    // thin_list< tstClass > lst = { 5, 4, 3, 99, };
    thin_list< int > lst1 = { 5, 4, 3, 99, 1, 4, 100, 100, 101 };
    thin_list< int > lst2;// = {  100,10, 12, 48, 15, 1, 3  };
    // lst1.splice(lst1.end(), lst2, ++lst2.begin(), --lst2.end() );
    // lst1.remove( 9 );
    // lst1.unique();
    // lst1.m_transfer( lst1.begin(), ++lst1.begin() );
    lst1.sort(std::greater< int >() );
    // lst1.remove_if( std::bind( myComp, std::placeholders::_1, 8 ) );
    for ( auto elem : lst1 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << "size = " << lst1.size() << std::endl;
    for ( auto elem : lst2 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << "size = " << lst2.size() << std::endl;
}

int main( void ) {
    test01();
    return 0;
}