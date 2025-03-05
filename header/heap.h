#ifndef __HEAP_H__
#define __HEAP_H__

#include "thin_vector.h"
#include"iterator_category_judgment.h"
namespace thinContainers {
    template < typename RandomAccessIterator, typename Distance, typename T, typename Compare >
    void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp );
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T* );

    template < typename RandomAccessIterator, typename Distance, typename T, typename Compare >
    void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value, Compare comp );
    template < typename RandomAccessIterator, typename Distance, typename T >
    void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value );

    template < typename RandomAccessIterator, typename T >
    void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T* );
    template < typename RandomAccessIterator, typename T, typename Compare >
    void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp );

    template < typename RandomAccessIterator, typename T, typename Distance >
    void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance* );
    template < typename RandomAccessIterator, typename T, typename Distance, typename Compare >
    void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp );

    template < typename RandomAccessIterator, typename Distance, typename T >
    void __adjust_heap( RandomAccessIterator first, Distance holeIndex, Distance len, T value );
    template < typename RandomAccessIterator, typename Distance, typename T, typename Compare >
    void __adjust_heap( RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp );

    template < typename RandomAccessIterator, typename T, typename Distance, typename Compare  >
    void __make_heap( RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare comp );
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __make_heap( RandomAccessIterator first, RandomAccessIterator last, T*, Distance*);

    template < typename RandomAccessIterator, typename T, typename Distance, typename Compare >
    void __make_heap_swin( RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare comp );
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __make_heap_swin( RandomAccessIterator first, RandomAccessIterator last, T*, Distance* );


    //push_heap算法
    //接受两个迭代器，表示一个heap底部容器vector的头尾。--'新元素已经插入到容器的最尾端'
    //如果不合符条件，push_heap的执行结果不可预期
    template < typename RandomAccessIterator,
    typename Compare,
    typename = typename std::enable_if< _is_random_access_iterator<RandomAccessIterator>::value >::type >
    inline void push_heap( RandomAccessIterator first, RandomAccessIterator last, 
        Compare comp ) {
        //此函数被调用时，新元素应该已置于底部容器的最尾端
        // __push_heap_aux( first, last, distance_type(first), value_type(first) );
        __push_heap_aux( first, last, (std::ptrdiff_t*)nullptr, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, comp );
    }
    template < typename RandomAccessIterator,
    typename = typename std::enable_if< _is_random_access_iterator<RandomAccessIterator>::value >::type >
    inline void push_heap( RandomAccessIterator first, RandomAccessIterator last ) {
        //此函数被调用时，新元素应该已置于底部容器的最尾端
        // __push_heap_aux( first, last, distance_type(first), value_type(first) );
        __push_heap_aux( first, last, (std::ptrdiff_t*)nullptr, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr );
    }

    template < typename RandomAccessIterator, typename Distance, typename T,
    typename Compare >
    inline void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp ) {
         thinContainers::__push_heap( first, Distance( (last-first) - 1 ), Distance(0), T( *(last - 1 ) ), comp  );
    }
    template < typename RandomAccessIterator, typename Distance, typename T >
    inline void __push_heap_aux( RandomAccessIterator first, RandomAccessIterator last, Distance*, T* ) {
        __push_heap( first, Distance( (last-first) - 1 ), Distance(0), T( *(last - 1 ) ) );
    }

    /*
    @brief:堆的上浮的过程，即往一个堆中插入元素。（前提是已经将这个元素放入到堆尾，元素之前是一个真正的堆）
    @first:堆顶元素的迭代器
    @holeIndex:push元素节点(即是最后一个元素last-1)的索引，以first迭代器为始(0)
    @topIndex:顶部节点的索引（迭代器first所指的元素）
    @value:push元素的值，*(last-1) 
    */
    template < typename RandomAccessIterator, typename Distance, typename T, 
    typename Compare >
    inline void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value, Compare comp ) {
        Distance parentIndex = (holeIndex - 1) / 2; //找出插入元素的父节点
        while ( holeIndex > topIndex && comp( *(first+parentIndex), value ) /* *(first + parentIndex) < value */ ) { 
            //尚未到达顶端，且父节点小于插入的值才进入循环
            //将插入的节点放到合适的位置，以符合大根堆的性质
            *( first + holeIndex ) = *( first + parentIndex );
            holeIndex = parentIndex;
            parentIndex = (holeIndex-1) / 2; 
        }
        *( first+holeIndex ) = value;
    }
    template < typename RandomAccessIterator, typename Distance, typename T >
    inline void __push_heap( RandomAccessIterator first, Distance holeIndex, Distance topIndex,const T& value ) {
        Distance parentIndex = (holeIndex - 1) / 2; //找出插入元素的父节点
        while ( holeIndex > topIndex && *(first + parentIndex) < value )  { 
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
    template < typename RandomAccessIterator, typename Compare,
    typename = typename std::enable_if<_is_random_access_iterator<RandomAccessIterator>::value>::type  >
    inline void pop_head( RandomAccessIterator first, RandomAccessIterator last, Compare comp ) {
        __pop_heap_aux( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, comp );
    }

    template < typename RandomAccessIterator, typename T >
    inline void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T* ) {
        __pop_heap( first, last - 1, last - 1, T(*(last-1)), (std::ptrdiff_t*)nullptr );
    }
    template < typename RandomAccessIterator, typename T, typename Compare >
    inline void __pop_heap_aux( RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp ) {
        __pop_heap( first, last - 1, last - 1, T(*(last-1)), (std::ptrdiff_t*)nullptr, comp );
    }

    /*
    @brief: 将pop的元素放到堆的末尾
    @param: first 堆顶元素
    @param：last 指向堆的最后一个元素的迭代器（切记：不是最后一个元素的下一个位置）
    @param：result 最后一个元素的迭代器
    @param：value 最后一个元素的值
    @param： Distance* 获取类型作用
     */
    template < typename RandomAccessIterator, typename T, typename Distance >
    inline void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result,  T value, Distance* ) {
        *result = *first; //将删除元素（即堆顶元素）的值存入堆的末尾
        __adjust_heap( first, Distance(0), Distance( last - first ), value );
    }
    template < typename RandomAccessIterator, typename T, typename Distance,typename Compare >
    inline void __pop_heap( RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result,  T value, Distance*, Compare comp ) {
        *result = *first; //将删除元素（即堆顶元素）的值存入堆的末尾
       thinContainers::__adjust_heap( first, Distance(0), Distance( last - first ), value, comp );
    }

    /*
    @brief:
    @param: first 堆顶元素
    @param: holeIndex 删除元素的索引（堆顶元素的索引）
    @param: len 删除一个元素后，需要处理的堆中的元素的个数。
    @param： value 原始堆的最后一个元素的值，即顶替堆顶元素进行下沉操作
     */
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
        __push_heap( first, holeIndex, topIndex, value );//将value放入到堆中。
    }

    template < typename RandomAccessIterator, typename Distance, typename T, typename Compare >
    void __adjust_heap( RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp ) {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2; //右子节点
        while ( secondChild < len ) {
            //比较两个左右子节点的值，然后以secondChild代表较大的子节点
            if ( comp( *(first+secondChild), *(first+secondChild-1)) ) { //*(first + secondChild) < *(first + secondChild - 1 )
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
        thinContainers::__push_heap( first, holeIndex, topIndex, value, comp );//将value放入到堆中。
    }

    /* 
    @brief：堆排序
    @param：first 堆顶迭代器
    @param： last 哨兵
    */
    template < typename RandomAccessIterator >
    void sort_heap( RandomAccessIterator first, RandomAccessIterator last ) {
        while ( last - first > 1) {
            pop_head( first, last-- );
        } 
    }
    template < typename RandomAccessIterator, typename Compare >
    void sort_heap( RandomAccessIterator first, RandomAccessIterator last, Compare comp ) {
        while ( last - first > 1) {
            pop_head( first, last--, comp );
        } 
    }

    /* 
    @brief: 将一个序列构建成堆heap
    */
    template < typename RandomAccessIterator,
    typename Compare >
    inline void make_heap( RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        // __make_heap( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, (std::ptrdiff_t*)nullptr );
        __make_heap_swin( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, (std::ptrdiff_t*)nullptr, comp );
    }
    template < typename RandomAccessIterator >
    inline void make_heap( RandomAccessIterator first, RandomAccessIterator last) {
        // __make_heap( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, (std::ptrdiff_t*)nullptr );
        __make_heap_swin( first, last, (typename std::iterator_traits< RandomAccessIterator >::value_type*)nullptr, (std::ptrdiff_t*)nullptr );
    }


    template < typename RandomAccessIterator, typename T, typename Distance, 
    typename Compare  >
    void __make_heap_swin( RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare comp ) {
        RandomAccessIterator currIter = first + 1;
        while ( currIter != last ) {
           thinContainers::push_heap( first, currIter, comp );
            ++currIter;
        }
       thinContainers:: push_heap( first, currIter, comp );
    }
    template < typename RandomAccessIterator, typename T, typename Distance >
    void __make_heap_swin( RandomAccessIterator first, RandomAccessIterator last, T*, Distance* ) {
        RandomAccessIterator currIter = first + 1;
        while ( currIter != last ) {
            push_heap( first, currIter);
            ++currIter;
        }
        push_heap( first, currIter );
    }


    /*
    @brief：应用下沉构建堆
    @param：first 序列首元素迭代器
    @param：last 哨兵
    @param：T* 标注类型
    @param：Distance* 标注类型 
     */
    //XXX: 下沉构建堆还没研究
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