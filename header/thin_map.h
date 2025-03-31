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
        using key_type = Key; //键值类型
        using data_type = Value; //实值类型
        using mapped_type = Value;// 实值类型
        using value_type = std::pair< const Key, Value >; //节点类型
        using key_compare = Compare;

    private:
        //数据成员--底层红黑树
        using rep_type =  rb_tree< key_type, value_type, __get_pair_first< key_type, Value >, key_compare, Alloc >; 
        rep_type t;
    public:
        using pointer = typename rep_type::pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::reference;
        using const_reference = typename rep_type::const_reference;
        using iterator = typename rep_type::iterator;
        using const_iterator = typename rep_type::const_iterator;
        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;

        //构造函数
       thin_map() : t( Compare() ) {}
        explicit thin_map( const Compare& comp ) : t( comp ) {}
        //范围构造函数
        template < typename InputIterator,
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        thin_map( InputIterator first, InputIterator last ) : t( Compare() ) {
            t.inset_unique( first, last );
        }
        template < typename InputIterator,
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        thin_map( InputIterator first, InputIterator last, const Compare& comp ) : t( comp ) {
            t.inset_unique( first, last );
        }
        //复制构造函数
        thin_map( const thin_map& other ) : t( other.t ) {}

        //赋值操作符
        thin_map& operator=( const thin_map& other ) {
            t = other.t;
            return *this;
        }

        key_compare key_comp() const {
            return t.key_comp();
        }
        iterator begin() {
            return t.begin();
        }
        iterator end() {
            return t.end();
        }
        const_iterator begin() const {
            return t.begin();
        }
        const_iterator end() const {
            return t.end();
        }
        bool empty() const {
            return t.empty();
        }
        size_type size() const {
            return t.size();
        }
        size_type max_size() const {
            return t.max_size();
        }

        Value& operator[]( const Key& k ) {
            return (*((insert( value_type(k, Value()))).first)).second;
        }
        void swap( thin_map& other ) {
            t.swap( other.t );
        }

        //插入函数：insert
        std::pair< iterator, bool > insert( const value_type& x ) {
            return t.insert_unique( x );
        }
        //todo：根据提示位置插入
        iterator insert( iterator hint, const value_type& x );
        template < typename InputIterator, 
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        void insert( InputIterator first, InputIterator last ) {
            t.insert_unique( first, last );
        }

        //删除函数：erase
        void erase( iterator position ) {
            t.erase( position );
        }
        size_type erase( const key_type& x ) {
            return t.erase( x );
        }
        void erase( iterator first, iterator last ) {
            t.erase( first, last );
        }
        void clear() {
            t.clear();
        }

        iterator find( const key_type& x ) {
            return t.find( x );
        }
        const_iterator find( const key_type& x ) const {
            return t.find( x );
        }
        size_type count( const key_type& x ) const {
            return t.count( x );
        }
        //todo:lower_bound()
        iterator lower_bound( const key_type& x );
        const_iterator lower_bound( const key_type& x ) const;
        //todo:upper_bound()
        iterator upper_bound( const key_type& x );
        const_iterator upper_bound( const key_type& x ) const;
        //todo:equal_range()
        std::pair< iterator, iterator > equal_range( const key_type& x );
        std::pair< const_iterator, const_iterator > equal_range( const key_type& x ) const;

        friend bool operator==( const thin_map& a, const thin_map& b ) {
            return a.t == b.t;
        }
        friend bool operator<( const thin_map& a, const thin_map& b ) {
            return a.t < b.t ;
        }
        

    }; //thin_map
} //thinContainers

#endif // __THIN_MAP_H__