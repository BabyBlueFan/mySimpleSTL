/* 2025.02.23 */

#ifndef __THIN_LIST_H__
#define __THIN_LIST_H__

#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>

namespace thinContainers {

    template < typename T >
    struct __list_node {
        T _m_data; //数据成员
        __list_node* _m_p_prev;
        __list_node* _m_p_next;

        // 构造节点，完美转发参数构造data
        template <typename... Args>
        __list_node(Args&&... args) : _m_data(std::forward<Args>(args)...), _m_p_prev(nullptr), _m_p_next(nullptr) {}
    }; //__list_node

    template < typename T > class __list_const_iterator;
    //记住：迭代器本质是指向元素的“指针” + 一系列的操作
    template < typename T >
    class __list_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag; //支持++ -- == !=
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
    private:
        __list_node< T >* _m_p_node;//数据成员
        //声明thin_list 是__list_node的友元可以访问其私有成员
        template < typename U,typename Alloc > friend class thin_list;
        // template < typename U > friend class __list_const_iterator;
    public:
        //构造函数  
        __list_iterator(__list_node<T>* node = nullptr) : _m_p_node(node) {}

        reference operator*() const {
            return _m_p_node->_m_data;
        }
        pointer operator->() const {
            return &(_m_p_node->_m_data);
        }
        //++操作
        __list_iterator& operator++() {
            _m_p_node = _m_p_node->_m_p_next;
            return *this;
        }
        __list_iterator operator++( int ) {
            __list_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_next;
            return temp;
        }
        //--操作
        __list_iterator& operator--() {
            _m_p_node = _m_p_node->_m_p_prev;
            return *this;
        }
        __list_iterator operator--(int) {
            __list_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_prev;
            return temp;
        }
        //== != 操作
        bool operator==( const __list_iterator& other ) const {
            return _m_p_node == other._m_p_node;
        }
        bool operator!=( const __list_iterator& other ) const {
            return _m_p_node != other._m_p_node;
        }
        //允许从非const迭代器转换成const迭代器
        operator __list_const_iterator< T >() const {
            return __list_const_iterator< T >( _m_p_node  );
        }
    }; //__list_iterator


    template < typename T >
    class __list_const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag; //支持++ -- == !=
        using value_type = const T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = ptrdiff_t;
    private:
        __list_node< T >* _m_p_node;//数据成员
        //声明thin_list 是__list_node的友元可以访问其私有成员
        template < typename U,typename Alloc > friend class thin_list;
    public:
        //构造函数  
        __list_const_iterator(__list_node<T>* node = nullptr) : _m_p_node(node) {}

        reference operator*() const {
            return _m_p_node->_m_data;
        }
        pointer operator->() const {
            return &(_m_p_node->_m_data);
        }
        //++操作
        __list_const_iterator& operator++() {
            _m_p_node = _m_p_node->_m_p_next;
            return *this;
        }
        __list_const_iterator operator++( int ) {
            __list_const_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_next;
            return temp;
        }
        //--操作
        __list_const_iterator& operator--() {
            _m_p_node = _m_p_node->_m_p_prev;
            return *this;
        }
        __list_const_iterator operator--(int) {
            __list_const_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_prev;
            return temp;
        }
        //== != 操作
        bool operator==( const __list_const_iterator& other ) const {
            return _m_p_node == other._m_p_node;
        }
        bool operator!=( const __list_const_iterator& other ) const {
            return _m_p_node != other._m_p_node;
        }
        /* //允许从非const迭代器转换成const迭代器
        operator __list_iterator< const T >() const {
            return __list_iterator< const T >( _m_p_node  );
        } */
    }; //__list_const_iterator

    template < typename T, typename Alloc = std::allocator< __list_node< T > > > 
    class thin_list {
    public:
        using iterator = __list_iterator< T >;
        using const_iterator = __list_const_iterator< T >;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using allocator_type = Alloc;
        using node_type = __list_node< T >;

        allocator_type get_allocator() {
            return allocator_type();
        }

    private:
        node_type*  m_p_sentinel;//哨兵
        size_type m_size;

    public:
        //默认构造函数
        thin_list() : m_p_sentinel( m_allocate() ), m_size( 0 ) {}
        //拷贝构造函数
        thin_list( const  thin_list& other ) : m_p_sentinel( m_allocate() ), m_size( 0 ) {
            for ( auto& elem : other ) {
                push_back( elem );
            }
        }

        //push_back()
        void push_back( const T& elem ) {
            node_type* ptr =  m_allocate();
            m_construct( ptr, elem );
            if ( 0 == m_size ) {
                m_p_sentinel->_m_p_prev = ptr;
                ptr->_m_p_next = m_p_sentinel;
                ptr->_m_p_prev = m_p_sentinel;
                m_p_sentinel->_m_p_next = ptr;
            } else {
                m_p_sentinel->_m_p_prev->_m_p_next = ptr;
                ptr->_m_p_prev = m_p_sentinel->_m_p_prev;
                m_p_sentinel->_m_p_prev = ptr;
                ptr->_m_p_next = m_p_sentinel;
            }
            ++m_size;
        }
        //size()
        size_type size() const {
            return m_size;
        }
        //begin()
        iterator begin() {
            return iterator( m_p_sentinel->_m_p_next );
        }
        const_iterator begin() const {
            return const_iterator( m_p_sentinel->_m_p_next );
        }
        iterator end() {
            return iterator( m_p_sentinel );
        }
        const_iterator end() const {
            return const_iterator( m_p_sentinel );
        }
    protected:
        //申请一个节点（__list_node< T >）的内存空间  
        node_type*  m_allocate( size_type n = 1 ) {
            return get_allocator().allocate( n );
        }
        //释放一个节点内存空间
        void m_deallocate( node_type* ptr, size_type n = 1 ) {
            get_allocator().deallocate( ptr , n );
        }
        //构造一个节点
        template < typename... Args >
        void m_construct( node_type* ptr, Args&&... args ) {
            get_allocator().construct( ptr, std::forward< node_type >(args)... );
        }
        //析构一个节点
        void m_destroy( node_type* ptr ) {
            get_allocator().destroy( ptr );
        }
    }; //thin_list

} //thinContainers

#endif //__THIN_LIST_H__