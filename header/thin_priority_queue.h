#ifndef __THIN_PRIORITY_QUEUE_H__
#define  __THIN_PRIORITY_QUEUE_H__

#include "thin_stack.h"
#include "heap.h"

namespace thinContainers {
    template < typename T, typename Container = thin_vector< T >, typename Compare = std::less< T > >
    class thin_priority_queue {
    public:
        using value_type = typename Container::value_type;
        using reference  = T&;
        using size_type  = typename Container::size_type;
        using const_reference = const T&;
    protected:
        Container c; //底层容器
        Compare comp; //元素比较标准
    public:
        thin_priority_queue() : c() {}
        explicit thin_priority_queue( const Compare& x ) : c(), comp(x) {}
        template < typename InputIterator >
        thin_priority_queue( InputIterator first, InputIterator last, const Compare& x ) 
        : c( first, last ), comp( x ) {
            thinContainers::make_heap(  c.begin(), c.end(), comp );
        }
        template < typename InputIterator >
        thin_priority_queue( InputIterator first, InputIterator last )
        : c( first, last ) {
           /*  for ( auto elem : c ) {
                std::cout << elem << std::endl;
            } */
            thinContainers::make_heap( c.begin(), c.end(), comp );
            /* for ( auto elem : c ) {
                std::cout << elem << std::endl;
            } */
        }
        bool empty() const {
            return c.empty();
        }
        size_type size() const {
            return c.size();
        }
        const_reference top() const {
            return c.front();
        }
        void push( const T& value ) {
            c.push_back( value );
            thinContainers::push_heap( c.begin(), c.end(), comp );
        }
        void pop() {
            thinContainers::pop_head( c.begin(), c.end(), comp );
            c.pop_back();
        }
    }; //thin_priority_queue
} //thinContainers

#endif // __THIN_PRIORITY_QUEUE_H__