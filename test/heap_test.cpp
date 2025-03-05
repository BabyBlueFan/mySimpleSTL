#include <iostream>
#include <algorithm>
#include <functional>

#include "../header/heap.h"

using namespace thinContainers;

int main( void ) {
    thin_vector< int > i_vec{ 9, 1, 2, 3, 4, 8, 9, 3, 5, };
    thinContainers::make_heap( i_vec.begin(), i_vec.end(), std::greater< int >() );
    for ( auto elem : i_vec ) {
        std::cout << elem << " "; // 9 5 8 3 4 0 2 3 1 
    }
    std::cout << std::endl;

    i_vec.push_back( 7 );
    thinContainers:: push_heap( i_vec.begin(), i_vec.end(), std::greater< int >() );
    for ( auto elem : i_vec ) {
        std::cout << elem << " "; // 9 7 8 3 5 0 2 3 1 4 
    }
    std::cout << std::endl;

    thinContainers:: pop_head( i_vec.begin(), i_vec.end(), std::greater< int >() );
    std::cout << i_vec.back() << std::endl;
    i_vec.pop_back();
    for ( auto elem : i_vec ) {
        std::cout << elem << " "; // 8 7 4 3 5 0 2 3 1 
    }
    std::cout << std::endl;

   thinContainers::sort_heap( i_vec.begin(), i_vec.end(), std::greater< int >() );
    for ( auto elem : i_vec ) {
        std::cout << elem << " "; // 0 1 2 3 4 5 6 7 8 
    }
    std::cout << std::endl;

}