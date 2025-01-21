/*2025.01.21
** C语言传函数指针
 */
#include <iostream>
using namespace std;

int fcmp( const void* elem1, const void* elem2 );

int main( void )
{
    int ia[10] = { 32, 92, 67, 58, 10, 4, 25, 52, 59, 54, };
    qsort( ia, sizeof(ia)/sizeof(ia[0]), sizeof(ia[0]), fcmp );
    for ( int i = 0; i != 10; ++i ) {
        cout << ia[i] << ' ';
    }
}

int fcmp( const void* elem1, const void* elem2 )
{
    const int* i1 = (const int*)elem1;
    const int* i2 = (const int*)elem2;
    if ( *i1 < *i2 ) {
        return -1;
    } else if ( *i1 == *i2 ) {
        return 0;
    } else {
        return 1;
    }

}