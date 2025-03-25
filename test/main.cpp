#include <iostream>
#include <functional>

void func_ptr( int a, int b, int c ) {
    std::cout << "this is a function pointer!\n" << a << " " << b << " " << c << "\n";
}

template < typename Functor >
void to_call_functor( Functor func ) {
    std::cout << "In to_call_functor!\n";
    func();
}

int main( void ) {
    auto ff = std::bind( func_ptr, 20, 20, 20 );
    std::function< void(void) > funcc = ff;
    to_call_functor( funcc );
    // funcc( 12, 14, 19 );
    return 0;
}

