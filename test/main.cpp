#include <iostream>
#include <functional>
#include <vector>
#include <type_traits>

template < typename RandomAccessIterator, typename Compare >
void _push_heap( RandomAccessIterator first, RandomAccessIterator last, Compare comp );

int main( void ) {
    std::vector < int > i_vec{ 1, 2, 3 , 4 };
    _push_heap( i_vec.begin(), i_vec.begin() + 1, std::greater< int >() ); //, std::greater< int >() ); 
    return 0; 
} 

template < typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits< RandomAccessIterator >::value_type>  >
void _push_heap( RandomAccessIterator first, RandomAccessIterator last, Compare comp /* = Compare() */) {
        // Compare comp;
        if ( comp( *first, *last ) ) {
            std::cout << "a < b " << std::endl;
        } else {
            std::cout << "a > b " << std::endl;
        }
    }