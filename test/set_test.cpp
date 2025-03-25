#include "../header/thin_set.h"
#include <iostream>
#include "../header/thin_vector.h"

using namespace thinContainers;

void test01( void ) {
    thin_vector< int > vec01{ 5, 9, 10, 45, 2, };
    thin_vector< int > vec02{ 5, 9, 10, 49, 2, };
    thin_set< int > set01{ vec01.begin(), vec01.end() };
    thin_set< int > set02( vec02.begin(), vec02.end() );
    std::cout << "size = " << set02.size() << std::endl;
   /*  auto iter = set02.begin();
    std::cout << *iter << std::endl; */
    for ( const auto elem : set01 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    // set01 = set02;
    for ( const auto elem : set02 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    if ( set01 < set01 ) {
        std::cout << "<" << std::endl;
    } else {
        std::cout << "!<" << std::endl;
    }
    return;
}

void test02( void ) {
    thin_vector< int > vec01{ 5, 9, 10, 45, 2, };
    thin_vector< int > vec02{ 5, 9, 12, 49, 100, };
    thin_set< int > set01{ vec01.begin(), vec01.end() };
    thin_set< int > set02( vec02.begin(), vec02.end() );
    for ( const auto elem : set01 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    // std::pair< thin_set< int >::iterator, bool > pp  =  set01.insert( 9 );
    // if ( pp.second ) {
    //     std::cout << *(pp.first) << std::endl;
    // }
    set01.insert( vec02.begin(), vec02.end() );
    for ( const auto elem : set01 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    /* thin_set< int >::iterator iter = set01.begin();
    ++iter;
    ++iter;
    set01.erase( iter ); */
    std::cout << set01.erase( 12 ) << std::endl;
    for ( const auto elem : set01 ) {
        std::cout << elem << " ";
    }
    std::cout << "-----------------" <<  std::endl;

    set01.swap( set02 );

    for ( const auto elem : set01 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    for ( const auto elem : set02 ) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return;
}

int main( void ) {
    test02();
    return 0;
}