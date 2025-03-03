#ifndef __THIN_STACK_H__
#define  __THIN_STACK_H__


#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>
#include "iterator_category_judgment.h"
#include "thin_deque.h"
#include "thin_list.h"

namespace thinContainers {

    template < typename T, typename Sequence = thin_deque< T > >
    class thin_stack {
    public:
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;
    protected:
        Sequence c;
    public:
        //利用 Sequence c的操作，完成stack的操作
        bool empty() const {
            return c.empty();
        }
        size_type size() const {
            return c.size();
        }
        reference top() {
            return c.back();
        }
        const_reference top() const {
            return c.back();
        }
        void push( const T& value ) {
            c.push_back( value );
        }
        void pop() {
            c.pop_back();
        }
       /*  friend bool operator==( const thin_stack&, const thin_stack& );
        friend bool operator<( const thin_stack&, const thin_stack& ); */
    };  //thin_stack

    /* template < typename T, typename Sequence >
    bool operator==( const thin_stack< T, Sequence >& x, const thin_stack< T, Sequence >& y ) {
        return x.c == y.c;
    } 

    template < typename T, typename Sequence >
    bool operator<( const thin_stack< T, Sequence >& x, const thin_stack< T, Sequence >& y ) {
        return x.c < y.c;
    }
 */
} //thinContainers


#endif // __THIN_STACK_H__