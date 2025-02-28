/* 2025.02.28 */
#include <iostream>
#include <algorithm>
#include <functional>
#include "../header/thin_deque.h"

using namespace thinContainers;

void test1( void ) {
    thin_deque< int > deq1( 63, 9 );
    for ( int i = 0; i != 63; ++i ) {
        deq1[i] = i + 1;
    }
    for ( int i = 0; i != 100; ++i ) {
        deq1.push_front( -i );
    }
    // deq1.push_front( 0 );
    // deq1.push_front( -1 );
    /* for ( int i = 64; i != 100; ++i ) {
        deq1.push_back( i );
    } */
    std::cout << deq1.size() << std::endl;
   /*  for ( size_t i = 0; i != deq1.size(); ++i ) {
        deq1[i] = i + 1;
    } */
    for ( auto elem : deq1 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main( void ) {
    test1();
    return 0;
}