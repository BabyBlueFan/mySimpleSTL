/* 2025.02.23 */

#ifndef __THIN_LIST_H__
#define __THIN_LIST_H__

#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>
#include "iterator_category_judgment.h"

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
    template < typename T > class __list_const_reverse_iterator;
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

    template < typename T >
    class __list_reverse_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag; //支持++ -- == !=
        using value_type =  T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
    private:
        __list_node< T >* _m_p_node;//数据成员
        //声明thin_list 是__list_node的友元可以访问其私有成员
        template < typename U,typename Alloc > friend class thin_list;
    public:
        //构造函数  
        __list_reverse_iterator(__list_node<T>* node = nullptr) : _m_p_node(node) {}

        reference operator*() const {
            return _m_p_node->_m_data;
        }
        pointer operator->() const {
            return &(_m_p_node->_m_data);
        }
        //--操作
        __list_reverse_iterator& operator--() {
            _m_p_node = _m_p_node->_m_p_next;
            return *this;
        }
        __list_reverse_iterator operator--( int ) {
            __list_reverse_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_next;
            return temp;
        }
        //++操作
        __list_reverse_iterator& operator++() {
            _m_p_node = _m_p_node->_m_p_prev;
            return *this;
        }
        __list_reverse_iterator operator++(int) {
            __list_reverse_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_prev;
            return temp;
        }
        //== != 操作
        bool operator==( const __list_reverse_iterator& other ) const {
            return _m_p_node == other._m_p_node;
        }
        bool operator!=( const __list_reverse_iterator& other ) const {
            return _m_p_node != other._m_p_node;
        }
        //允许从非const迭代器转换成const迭代器
        operator __list_const_reverse_iterator< T >() const {
            return __list_const_reverse_iterator< T >( _m_p_node  );
        }

    }; //__list_reverse_iterator

    template < typename T >
    class __list_const_reverse_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag; //支持++ -- == !=
        using value_type =  const T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = ptrdiff_t;
    private:
        __list_node< T >* _m_p_node;//数据成员
        //声明thin_list 是__list_node的友元可以访问其私有成员
        template < typename U,typename Alloc > friend class thin_list;
    public:
        //构造函数  
        __list_const_reverse_iterator(__list_node<T>* node = nullptr) : _m_p_node(node) {}

        reference operator*() const {
            return _m_p_node->_m_data;
        }
        pointer operator->() const {
            return &(_m_p_node->_m_data);
        }
        //--操作
        __list_const_reverse_iterator& operator--() {
            _m_p_node = _m_p_node->_m_p_next;
            return *this;
        }
        __list_const_reverse_iterator operator--( int ) {
            __list_const_reverse_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_next;
            return temp;
        }
        //++操作
        __list_const_reverse_iterator& operator++() {
            _m_p_node = _m_p_node->_m_p_prev;
            return *this;
        }
        __list_const_reverse_iterator operator++(int) {
            __list_const_reverse_iterator temp = *this;
            _m_p_node = _m_p_node->_m_p_prev;
            return temp;
        }
        //== != 操作
        bool operator==( const __list_const_reverse_iterator& other ) const {
            return _m_p_node == other._m_p_node;
        }
        bool operator!=( const __list_const_reverse_iterator& other ) const {
            return _m_p_node != other._m_p_node;
        }
        //允许从非const迭代器转换成const迭代器
        /* operator __list_const_iterator< T >() const {
            return __list_const_iterator< T >( _m_p_node  );
        } */

    }; //__list_const_reverse_iterator

    template < typename T, typename Alloc = std::allocator< __list_node< T > > > 
    class thin_list {
    public:
        using iterator = __list_iterator< T >;
        using const_iterator = __list_const_iterator< T >;
        using reverse_iterator = __list_reverse_iterator< T >;
        using const_reverse_iterator = __list_const_reverse_iterator< T >;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using allocator_type = Alloc;
        using node_type = __list_node< T >;

        allocator_type get_allocator() const noexcept {
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
        //移动构造函数
        thin_list( thin_list&& other ) {
            m_p_sentinel = other.m_p_sentinel;
            m_size = other.m_size;
            other.m_p_sentinel = nullptr; other.m_size = 0;
        }
        //填充构造函数
        explicit thin_list( size_type cnt, const T& value) : thin_list() {
            for ( size_type i = 0; i != cnt; ++i ) {
                push_back( value );
            }
        }
        //初始化列表构造
        thin_list( const std::initializer_list< T >& initlst ) : thin_list() {
            auto iter = initlst.begin();
            while ( iter != initlst.end() ) {
                push_back( *(iter++) );
            }
        }
        //范围构造函数
        template < typename InputIter, 
                   typename std::enable_if< _is_input_iterator< InputIter >::value>::type* = nullptr > 
        thin_list(InputIter first, InputIter last) : thin_list() {
            auto iter = first;
            while ( iter != last ) {
                push_back( *(iter++) );
            }
        }

        //析构函数 
        ~thin_list() {
            // m_destroy()
            node_type* ptr = m_p_sentinel->_m_p_next;
            while ( ptr != m_p_sentinel ) {
                m_destroy( ptr );
                ptr = ptr->_m_p_next;
            }
            ptr = m_p_sentinel->_m_p_next;
            // m_deallocate( ptr, m_size );
            while ( ptr != m_p_sentinel ) {
                m_deallocate( ptr );
                ptr = ptr->_m_p_next;
            }
            m_deallocate( ptr );
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
        // 在尾部就地构造元素（C++11起）
        template < typename... Args >
        void emplace_back( Args&&... args ) {
            node_type* ptr =  m_allocate();
            m_construct( ptr, std::forward< Args >(args)... );
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
        //头部插入元素
        void push_front( const T& elem ) {
            node_type* ptr = m_allocate();
            m_construct( ptr, elem );
            if ( 0 == m_size ) {
                ptr->_m_p_prev = m_p_sentinel;
                ptr->_m_p_next = m_p_sentinel;
                m_p_sentinel->_m_p_next = ptr;
                m_p_sentinel->_m_p_prev = ptr;
            } else {
                m_p_sentinel->_m_p_next->_m_p_prev = ptr;
                ptr->_m_p_prev = m_p_sentinel;
                ptr->_m_p_next = m_p_sentinel->_m_p_next;
                m_p_sentinel->_m_p_next = ptr;
            }
            ++m_size;
        }
        // 在头部就地构造元素（C++11起，避免拷贝）
        template < typename... Args >
        void emplace_front( Args&&... args ) {
            node_type* ptr = m_allocate();
            m_construct( ptr, std::forward<Args>(args)... );
            if ( 0 == m_size ) {
                ptr->_m_p_prev = m_p_sentinel;
                ptr->_m_p_next = m_p_sentinel;
                m_p_sentinel->_m_p_next = ptr;
                m_p_sentinel->_m_p_prev = ptr;
            } else {
                m_p_sentinel->_m_p_next->_m_p_prev = ptr;
                ptr->_m_p_prev = m_p_sentinel;
                ptr->_m_p_next = m_p_sentinel->_m_p_next;
                m_p_sentinel->_m_p_next = ptr;
            }
            ++m_size;
        }
        // 删除尾部元素（列表为空时行为未定义）
        void pop_back() {
            node_type* pop_node = m_p_sentinel->_m_p_prev;
            m_p_sentinel->_m_p_prev = pop_node->_m_p_prev;
            pop_node->_m_p_prev->_m_p_next = m_p_sentinel;
            m_destroy( pop_node );
            m_deallocate( pop_node , 1 );
            pop_node = nullptr;// pop_node->_m_p_next = nullptr; pop_node->_m_p_prev = nullptr;
            --m_size;
        }
        // 删除头部元素（列表为空时行为未定义）
        void pop_front() {
            node_type* pop_node = m_p_sentinel->_m_p_next;
            m_p_sentinel->_m_p_next = pop_node->_m_p_next;
            pop_node->_m_p_next->_m_p_prev = m_p_sentinel;
            --m_size;
            m_destroy( pop_node );
            m_deallocate( pop_node ); pop_node = nullptr;
        }
        //size()
        size_type size() const noexcept {
            return m_size;
        }
        //max_size()
        size_type max_size() const noexcept {
            return ( std::allocator_traits< allocator_type >::max_size( get_allocator() ) );
        }
        //调整大小：cnt > size()，添加默认构造的元素；否则删除多余元素
        void resize( size_type cnt ) {
            if ( cnt > m_size ) {
                while ( cnt !=  m_size ) {
                    push_back( T() );
                }
            } else {
                while ( cnt != m_size ) {
                    pop_back();
                }
            }
        }
        //调整列表大小：若 cnt > size()，添加value的副本；否则删除多余元素
        void resize( size_type cnt, const T& value ) {
            if ( cnt > m_size ) {
                while ( cnt != m_size ) {
                    push_back( value );
                }
            } else {
                while ( cnt != m_size ) {
                    pop_back();
                }
            }
        }
        //begin()
        iterator begin() noexcept {
            return iterator( m_p_sentinel->_m_p_next );
        }
        const_iterator begin() const noexcept {
            return const_iterator( m_p_sentinel->_m_p_next );
        }
        const_iterator cbegin() const noexcept {
            return const_iterator( m_p_sentinel->_m_p_next );
        }
        //rbegin()
        reverse_iterator rbegin() noexcept {
            return reverse_iterator(m_p_sentinel->_m_p_prev);
        }
        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator( m_p_sentinel->_m_p_prev );
        }
        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator( m_p_sentinel->_m_p_prev );
        }
        //end()
        iterator end() noexcept {
            return iterator( m_p_sentinel );
        }
        const_iterator end() const noexcept {
            return const_iterator( m_p_sentinel );
        }
        const_iterator cend() const noexcept {
            return const_iterator( m_p_sentinel );
        }
        //rend()
        reverse_iterator rend() noexcept {
            return reverse_iterator( m_p_sentinel );
        }
        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator( m_p_sentinel );
        }
        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator( m_p_sentinel );
        }
        //front()返回首元素的引用（列表为空时行为未定义）
        reference front() {
           return  (*begin());
        }
        const_reference front() const {
            return (*cbegin());
        }
        // 返回尾元素的引用（列表为空时行为未定义）
        reference back() {
            // return (m_p_sentinel->_m_p_prev->_m_data);
            return *( --end() );
        }
        const_reference back() const {
            return *(--cend());
        }
        //empry()
        bool empty() const noexcept {
            return ( 0 == m_size );
        }
    protected:
        //申请一个节点（__list_node< T >）的内存空间  
        inline node_type*  m_allocate( size_type n = 1 ) {
            return get_allocator().allocate( n );
        }
        //释放一个节点内存空间
        inline void m_deallocate( node_type* ptr, size_type n = 1 ) {
            get_allocator().deallocate( ptr , n );
        }
        //构造一个节点
        template < typename... Args >
        inline void m_construct( node_type* ptr, Args&&... args ) {
            get_allocator().construct( ptr, std::forward< node_type >(args)... );
        }
        //析构一个节点
        inline void m_destroy( node_type* ptr ) {
            get_allocator().destroy( ptr );
        }
    }; //thin_list

} //thinContainers

#endif //__THIN_LIST_H__