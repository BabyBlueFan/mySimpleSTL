#ifndef __THIN_MAP_H__
#define  __THIN_MAP_H__

#include "./rb_tree.h"

namespace thinContainers {
    template < typename T1, typename T2 >
    struct __get_pair_first
    {  
        T1 operator()( const std::pair< T1, T2 >& arg ) {
            return arg.first;
        } 
    };
    
    template < typename Key, typename Value, 
    typename Compare = std::less< Key >, 
    typename Alloc = std::allocator< __rb_tree_node< std::pair< const Key, Value > > > >
    class thin_map {
    public:
        using key_type = Key;
        using data_type = Value;
        using mapped_type = Value;
        using value_type = std::pair< const Key, Value >;
        using key_compare = Compare;

    private:
        //数据成员--底层红黑树
        using rep_type =  rb_tree< key_type, value_type, __get_pair_first, key_compare, Alloc >; 
        rep_type t;
    }; //thin_map


} //thinContainers

#endif // __THIN_MAP_H__