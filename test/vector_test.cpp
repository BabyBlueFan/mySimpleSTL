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

class tstClass {
private:
    int a;
    double b;
    
public:
    tstClass( int _a = 0, double _b = 0 ) : a(_a), b(_b) {
        cout << "tstClass 默认构造\n"; 
    }
    ~tstClass() {
        cout << "tstClass 析构\n";
    }
    friend std::ostream& operator<<(std::ostream& os, const tstClass& obj);
};

std::ostream& operator<<( std::ostream& os, const tstClass& obj ) {
    os << obj.a << " and " << obj.b ;
    return os;
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

//赋值操作符
void test02( void ) {
    thin_vector< tstClass > vec1 = { tstClass(), tstClass(), tstClass() };
    thin_vector< tstClass > vec2 = { tstClass( 5, 55 ), tstClass( 6,66) };
    printVec( vec2 );
    vec2 = { tstClass( 1, 1 ), tstClass(1, 1), tstClass(1, 1 ) };
    printVec( vec2 );
    vec2 = std::move( vec1 );
    printVec( vec1 );
    printVec( vec2 );
    cout << "size = " << vec2.size() << " cap = " << vec2.capacity() << endl;
    vec2.at(0) = tstClass( 4, 4 );
    vec2[1] = tstClass( 8, 8 );
    vec2.at(2) = tstClass( 5, 5 );
    printVec( vec2 );
    cout << vec2.front() << endl;
    cout << vec2.back() << endl;
    vec2.back() = tstClass( 100, 88 );
    printVec( vec2 );
    // auto p = vec2.data();
    // vec2.at(3) = tstClass( 6, 6 );
}

//容量相关操作
void test03( void ) {
    thin_vector< int > i_vec{ 8, 7, 6 };
    cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    printVec( i_vec );
    const auto iter = i_vec.begin();
    // iter = iter + 2;
    std::list< int > lst = { 1, 2, 3, 4, 5 };
    i_vec.insert( iter+2, { 44, 55, 66 } );
    cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    printVec( i_vec );
    // i_vec.shrink_to_fit();
    // cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    // i_vec.reserve( 100 );
    // printVec( i_vec );
    // cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    // i_vec.shrink_to_fit();
    // cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    // thin_vector< int > ii_vec{ 100 };
    // cout << "size = " << ii_vec.size() << " capacity = " << ii_vec.capacity() << endl;
    // i_vec.reserve( 55 );
    // ii_vec.swap( i_vec );
    // printVec( i_vec );
    // printVec( ii_vec );
    // cout << "size = " << i_vec.size() << " capacity = " << i_vec.capacity() << endl;
    // cout << "size = " << ii_vec.size() << " capacity = " << ii_vec.capacity() << endl;
    // thin_vector< int >( ii_vec ).swap(ii_vec );
    // printVec( ii_vec );
    // cout << "size = " << ii_vec.size() << " capacity = " << ii_vec.capacity() << endl;
}

int main( void ) {
    test03();
    return 0;
}