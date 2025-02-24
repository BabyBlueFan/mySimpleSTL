/* 2025.02.23 
**迭代器类型标签
**迭代器公共接口？？
*/

#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <iostream>


namespace thinContainers {
    // 判断是否是输入迭代器
    template< typename T, typename = void >
    struct _is_input_iterator : std::false_type {};
    template < typename T >
    struct _is_input_iterator< T, 
                        std::void_t< typename std::iterator_traits< T >::iterator_category::input_iterator_tag > 
                        >: std::true_type {};

#if 0
    //迭代器的五种类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    //迭代器类模板
    template < typename Category, typename T, 
            typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T& >
    struct iterator {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };


#endif
} //thinContainers




#endif //__ITERATOR_H__
