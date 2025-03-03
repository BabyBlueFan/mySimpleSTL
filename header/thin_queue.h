#ifndef __THIN_QUEUE_H__
#define __THIN_QUEUE_H__

#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>
#include "iterator_category_judgment.h"
#include "thin_deque.h"
#include "thin_list.h"

namespace thinContainers {

    template < typename T, typename Container = thin_deque< T > >
    class thin_queue {
    public:
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
    protected:
        Container c;
    public:
        friend bool operator==( const thin_queue& x, const thin_queue& y ) {
            return x.c == y.c;
        }
        bool empty() const {
            return c.empty();
        }
        size_type size() const {
            return c.size();
        }
        reference front() {
            return c.front();
        }
        const_reference front() const {
            return c.front();
        }
        reference back() {
            return c.back();
        }
        const_reference back() const {
            return c.back();
        }
        void push( const T& value ) {
            c.push_back( value );
        }
        void pop() {
            c.pop_front();
        }

    };


} // thinContainers

#endif //__THIN_QUEUE_H__