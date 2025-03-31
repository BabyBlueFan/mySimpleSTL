#ifndef __THIN_SET_H__
#define __THIN_SET_H__

#include "./rb_tree.h"
#include "./iterator_category_judgment.h"

namespace thinContainers {

    template < typename T >
    struct __set_keyTovalue {
        T operator()( const T& x ) {
            return x;
        }
    };

    template < typename Key, typename Compare = std::less< Key >, typename Alloc = std::allocator< __rb_tree_node<Key> > >
    class thin_set {
    public:
        using allocator_type = Alloc;
        allocator_type get_allocator() const {
            return allocator_type();
        } 
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;

    private:
        using rep_type = rb_tree< key_type, value_type, __set_keyTovalue< value_type >, key_compare, Alloc >;
        //数据成员
        rep_type t;

    public:
        using pointer = typename rep_type::const_pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::const_reference;
        using const_reference = typename rep_type::const_reference;
        using iterator = typename rep_type::const_iterator; //set的迭代器不允许进行写操作typename rep_type::const_reference;
        using const_iterator = typename rep_type::const_iterator; //set的迭代器不允许进行写操作typename rep_type::const_reference;
        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;
        //todo:reverse_iterator
        /* using reverse_iterator = typename rep_type::const_reverse_iterator;
        using const_reverse_iterator = typename rep_type::const_reverse_iterator; */

        //默认构造函数
        thin_set() : t( Compare() ) {}
        explicit thin_set( const Compare& comp ) : t( comp ) {}
        //范围构造函数
        template < typename InputIterator,
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        thin_set( InputIterator first, InputIterator last ) : t( Compare() ) {
            t.insert_unique( first, last );
        }
        template < typename InputIterator, 
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        thin_set( InputIterator first, InputIterator last, const Compare& comp  ) : t( comp ) {
            t.insert_unique( first, last );
        }
        //复制构造函数
        thin_set( const thin_set& other ) : t( other.t ) {} 
        /* ~thin_set() {
            t.~rb_tree();
        } */

        //赋值操作符 
        thin_set& operator=( const thin_set& other ) {
            t = other.t;
            return *this;
        }

        /***********************************************************************************/
        //一下操作通过rb_tree实现
        key_compare key_comp() const {
            return t.key_comp(); 
        }
        value_compare value_comp() const {
            return t.key_comp();
        }
        iterator begin() {
            return t.begin();
        }
        iterator end() {
            return t.end();
        }
        //todo:需要rb_tree的反向迭代器
        /* reverse_iterator rbegin() const {
            return t.rbegin();
        }
        reverse_iterator rend() const {
            return t.rend();
        } */
        bool empty() const {
            return t.empty();
        }
        size_type size() const {
            return t.size();
        }
        size_type max_size() const {
            return t.max_size();
        }
        void swap( thin_set& other ) {
            // t.clear();
            t.swap(other.t);
        }
        /***********************************************************************************/
        using pair_iterator_bool = std::pair< iterator, bool >;
        //插入元素
        std::pair< iterator, bool > insert( const value_type& x ) {
            std::pair< typename rep_type::iterator, bool > p = t.insert_unique( x );
            return std::make_pair( p.first, p.second );
        }
        //todo:给出位置插入，红黑树的对应实现
        /* iterator insert( iterator position, const value_type& x ) {
            using rep_iterator = typename rep_type::iterator;
            return t.insert_unique( (rep_iterator&)position, x );
        } */
        template < typename InputIterator, 
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        void insert( InputIterator first, InputIterator last ) {
            t.insert_unique( first, last );
        }

        //删除
        void erase( iterator position ) {
            using rep_iterator = typename rep_type::iterator;
            t.erase( (rep_iterator&)position );
        }
        size_type erase( const key_type& x ) {
            return t.erase( x );
        }
        void erase( iterator first, iterator last ) {
            using rep_iterator = typename rep_type::iterator;
            t.erase( (rep_iterator&)first, (rep_iterator&)last );
        }

        void clear() {
            t.clear();
        }

        iterator find( const key_type& x ) const {
            return t.find( x );
        }
        size_type count( const key_type& x ) const {
            return t.count( x );
        }
        //todo: 红黑树 lower_bound()
        iterator lower_bound( const key_type& x ) const {
            return t.lower_bound(x);
        }
        //todo: 红黑树 upper_bound()
        iterator upper_bound( const key_type& x ) const {
            return t.upper_bound( x );
        }
        //todo: 红黑树 equal_range()
        std::pair< iterator, iterator > equal_range( const key_type& x ) const {
            return t.equal_range( x );
        }

        //操作符重载
        //== 
        friend bool operator==( const thin_set& a, const thin_set& b ) {
            return a.t == b.t;
        }
        //<
        friend bool operator<( const thin_set& a, const thin_set& b ) {
            return a.t < b.t;
        }

    }; //thin_set
} //thinContainers

#endif //__THIN_SET_H__