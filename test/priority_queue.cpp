#include "../header/thin_priority_queue.h"

using namespace thinContainers;

int main( void ) {
    int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5};
    thin_priority_queue<int, thin_vector<int> > ipq( ia, ia+9 );
    std::cout << "size = " << ipq.size() << std::endl;

    for ( int i = 0; i < ipq.size(); ++i ) {
        std::cout << ipq.top() << " ";
    }
    std::cout << std::endl;
    while ( !ipq.empty() ) {
        std::cout << ipq.top() << " ";
        ipq.pop();
    }
    std::cout << std::endl;
    return 0;
}