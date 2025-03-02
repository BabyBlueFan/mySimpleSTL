/* 2025.02.28 */
#include <iostream>
#include <algorithm>
#include <functional>
#include "../header/thin_deque.h"

using namespace thinContainers;

void func( thin_deque< int >::const_iterator con_iter ) {
    std::cout << "HelloWorld!\n";
    return;
}

void test1( void ) {
    thin_deque< int > deq1( 1, 9 );
    thin_deque < int >::const_iterator iter = deq1.begin();
    std::cout << *iter << std::endl;
    // *iter = 100;
    deq1.push_back( 10 );
    deq1.push_back( 100 );
    ++iter;
    std::cout << *iter << std::endl;
    ++iter;
    std::cout << *iter << std::endl;
    auto it = std::find( deq1.begin(), deq1.end(), 10 );
    std::cout << *it << std::endl;
    for ( auto elem : deq1 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main( void ) {
    test1();
    thin_deque< int > de01( 3, 66 );
    func( de01.begin() );
    return 0;
}