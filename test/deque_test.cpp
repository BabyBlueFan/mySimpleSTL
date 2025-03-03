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

class myClass {
public:
    int m_a;
    myClass( int a = 0 ) : m_a( a ) {
        std::cout << m_a << " 构造" << std::endl;
    }
    ~myClass() {
        std::cout << m_a << " 析构" << std::endl;
    }
};


void test1( void ) {
    thin_deque< myClass > deq1( 13, 9 );
    int i = 1;
    for ( auto it = deq1.begin(); it != deq1.end(); ++it ) {
        *it = myClass( i++ );
    }
    for ( auto elem : deq1 ) {
        std::cout << elem.m_a << " ";
    }
    std::cout << std::endl;
    /* deq1.pop_front();
    deq1.pop_front();
    deq1.pop_front();
    deq1.pop_front();
    deq1.pop_front();
    deq1.pop_front();
    deq1.pop_front(); */
    std::cout << "****************************"<< std::endl;
    deq1.clear();
    deq1.clear();
    std::cout << "****************************"<< std::endl;
    for ( auto elem : deq1 ) {
        std::cout << elem.m_a << " ";
    }
    std::cout << std::endl;
}

void test02( void ) {
    thin_deque < myClass > de1( 60, 0 );
    int i = 1;
    for ( auto it = de1.begin(); it != de1.end(); ++it ) {
        *it = myClass( i++ );
    }
    auto iter = de1.begin();//de1.begin() + 7;
    // iter = de1.erase( iter );
    iter =  de1.erase( iter+18, iter+29 );
    if ( iter == de1.end() ) {
        std::cout << "end!\n";
    } else {

        std::cout << "返回迭代器: " << (*iter).m_a << std::endl;
    }
    std::cout << "****************************"<< std::endl;
    for ( auto elem : de1 ) {
        std::cout << elem.m_a << " ";
    }
    std::cout << std::endl;

}

void test03( void ) {
    thin_deque < myClass > de1( 60, 5 );
    // std::cout << (*( de1.insert( de1.end(), myClass(10) ) )).m_a << std::endl;;
    int i = 1;
    for ( auto it = de1.begin(); it != de1.end(); ++it ) {
        *it = myClass( i++ );
    }
    auto iter = de1.insert( de1.begin() + 56, 100 );
    std::cout << "返回的迭代器： " << (*iter).m_a  << std::endl; 
    for ( auto elem : de1 ) {
        std::cout << elem.m_a << " ";
    }
    std::cout << std::endl;

    return;
}

int main( void ) {
    test03();
    // thin_deque< int > de01( 3, 66 );
    // func( de01.begin() );
    return 0;
}