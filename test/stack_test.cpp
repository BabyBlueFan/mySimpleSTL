/* 2025.03.03 */
#include "../header/thin_stack.h"


using namespace thinContainers;
int main( void ) {

    thin_stack< int, thin_list<int>  > lst_stack;
    lst_stack.push( 100 );
    lst_stack.push( 999 );
    std::cout << lst_stack.top() << std::endl;
    lst_stack.pop();
    std::cout << lst_stack.top() << std::endl;
    return 0;
}