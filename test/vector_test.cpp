#include <iostream>
#include <vector>
#include "../header/thin_vector.h"
#include <algorithm>
#include <list>
#include <string>

using std::string;
using std::cout;
using std::endl;

using namespace thinContainers;

template < typename T >
void printVec( thin_vector< T >& arg ) {
    for( auto elem : arg ) {
        cout << elem << " ";
    }
    cout << endl;
    return;
}

//测试构造函数
void test01( void ) {
    //测试填充构造函数
    thin_vector< int > vec1( 5 );
    printVec( vec1 );
    thin_vector< int > vec2( 6, 99 );
    printVec( vec2 );
    //范围构造函数测试
    std::list< int > lst = { 1, 5, 7, 1 };
    thin_vector< int > vec3( ++lst.begin(), lst.end() );
    printVec( vec3 );
    //移动构造函数测试
    thin_vector< int > vec4( std::move( vec3 ) );
    printVec( vec4 );
    printVec( vec3 );
    //初始化列表测试
    thin_vector< int > vec5 = { 8, 50, 3, 8, 1, };
    printVec( vec5 );
    return;
}

int main( void ) {
    test01();
    return 0;
}