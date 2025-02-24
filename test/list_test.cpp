/* 2025.02.23 */

#include "../header/thin_list.h"
#include <iostream>
#include <algorithm>

using namespace thinContainers;

void test01( void ) {
    thin_list< int > lst;
    lst.push_back( 8 );
    lst.push_back( 9 ); 
    lst.push_back( 10 ); 
    lst.push_back( 3 ); 
    lst.push_back( 2 );
    for ( auto elem : lst ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl << "size = " << lst.size() << std::endl;
    // thin_list< int >::const_iterator iter = lst.begin();
    thin_list< int > lst1(lst);
    lst1.push_back( 100 );
    for ( auto elem : lst1 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl << "size = " << lst1.size() << std::endl;
    thin_list< int >::const_iterator iter = lst1.begin();
    std::cout << *iter << std::endl;
    return;
}

int main( void ) {
    test01();
    return 0;
}