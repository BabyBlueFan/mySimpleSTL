#ifndef __HEAP_H__
#define __HEAP_H__

#include "thin_vector.h"
#include"iterator_category_judgment.h"
namespace thinContainers {
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T* );
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value );
    template < typename RandomAccessIterator, typename T >
    void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T* );
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance* );
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __adjust_heap( RandomAccessIterator first, Distance holeIndex, Distance len, T value );
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __make_heap( RandomAccessIterator first, RandomAccessIterator last, T*, Distance* );


    //push_heap算法
    //接受两个迭代器，表示一个heap底部容器vector的头尾。--'新元素已经插入到容器的最尾端'
    //如果不合符条件，push_heap的执行结果不可预期
    template < typename RandomAccessIterator,
    typename = typename std::enable_if< _is_random_access_iterator<RandomAccessIterator>::value >::type >
    inline void push_heap( RandomAccessIterator first, RandomAccessIterator last ) {
        //此函数被调用时，新元素应该已置于底部容器的最尾端
        // __push_heap_aux( first, last, distance_type(first), value_type(first) );
        __push_heap_aux( first, last, (std::ptrdiff_t*)nullptr, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr );
    }

    template < typename RandomAccessIterator, typename Distance, typename T >
    inline void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T* ) {
        __push_heap( first, Distance( (last-first) - 1 ), Distance(0), T( *(last - 1 ) )  );
    }

    /*@holeIndex:插入节点的索引
    **@topIndex:顶部节点的索引 
    */
    template < typename RandomAccessIterator, typename Distance, typename T >
    inline void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value ) {
        Distance parentIndex = (holeIndex - 1) / 2; //找出父节点
        while ( holeIndex > topIndex && *(first + parentIndex) < value ) { 
            //尚未到达顶端，且父节点小于插入的值才进入循环
            //将插入的节点放到合适的位置，以符合大根堆的性质
            *( first + holeIndex ) = *( first + parentIndex );
            holeIndex = parentIndex;
            parentIndex = (holeIndex-1) / 2; 
        }
        *( first+holeIndex ) = value;
    }


    template < typename RandomAccessIterator,
    typename = typename std::enable_if<_is_random_access_iterator<RandomAccessIterator>::value>::type  >
    inline void pop_head( RandomAccessIterator first, RandomAccessIterator last ) {
        __pop_heap_aux( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr );
    }
    template < typename RandomAccessIterator, typename T >
    inline void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T* ) {
        __pop_heap( first, last - 1, last - 1, T(*(last-1)), (std::ptrdiff_t*)nullptr );
    }
    template < typename RandomAccessIterator, typename T, typename Distance >
    inline void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance* ) {
        *result = *first;
        __adjust_heap( first, Distance(0), Distance( last - first ), value );
    }
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __adjust_heap( RandomAccessIterator first, Distance holeIndex, Distance len, T value ) {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2; //右子节点
        while ( secondChild < len ) {
            //比较两个左右子节点的值，然后以secondChild代表较大的子节点
            if ( *(first + secondChild) < *(first + secondChild - 1 ) ) {
                --secondChild;
            }
            *(first + holeIndex )  = *( first + secondChild );
            holeIndex = secondChild;
            secondChild = 2 * (secondChild + 1 );
        }
        if ( secondChild == len ) { //没有右子节点，只有左子节点
            *(first + holeIndex) = *( first + (secondChild-1) );
            holeIndex = secondChild - 1;
        }
        __push_heap( first, holeIndex, topIndex, value );
    }

    template < typename RandomAccessIterator >
    void sort_heap( RandomAccessIterator first, RandomAccessIterator last ) {
        while ( last - first > 1) {
            pop_head( first, last-- );
        } 
    }

    //[ first, last ),排列为一个heap
    template < typename RandomAccessIterator >
    inline void make_heap( RandomAccessIterator first, RandomAccessIterator last ) {
        __make_heap( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, (std::ptrdiff_t*)nullptr );
    }
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __make_heap( RandomAccessIterator first, RandomAccessIterator last, T*, Distance* ) {
        if ( last - first < 2 ) { //如果长度为0或者为1，不必重新排列
            return;
        } 
        Distance len = last - first;
        Distance parent = (len - 2) / 2;
        while ( true ) {
            __adjust_heap( first, parent, len, T(*(first+parent)));
            if ( parent == 0 ) {
                return;
            }
            --parent;
        }
    }

} //thinContainers

#endif //__HEAP_H__