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
#include "thin_vector.h"

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
        thin_list() : m_p_sentinel( m_allocate() ), m_size( 0 ) {
            m_p_sentinel->_m_p_next = m_p_sentinel;
            m_p_sentinel->_m_p_prev = m_p_sentinel;
        }
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
        // 拷贝赋值：用 other 的内容替换当前列表
        thin_list& operator=( const thin_list& other ) {
            assgin( other.begin(), other.end() );
            return *this;
        }
        // 移动赋值：将 other 的资源移动到当前列表
        thin_list& operator=( thin_list&& other ) noexcept {
            thin_vector< iterator > tmpVce;
            if ( 0 != m_size ) {
                node_type* ptr = m_p_sentinel->_m_p_next;
                iterator tmpIter = iterator(ptr);
                while ( tmpIter != end() ) {
                    tmpVce.push_back( tmpIter++ );
                }
                // tmpIter = iterator( ptr );
                for ( auto elem : tmpVce ) {
                    m_destroy( elem._m_p_node );
                    m_deallocate( elem._m_p_node );
                }
            }
            m_size = other.m_size; other.m_size = 0;
            m_p_sentinel = other.m_p_sentinel; other.m_p_sentinel = nullptr;
            return *this;
        }
        // 用初始化列表 `ilist` 的内容赋值（C++11起）
        thin_list& operator=( std::initializer_list< T > lst ) {
            assgin( lst.begin(), lst.end() );
            return *this;
        }
        //析构函数 
        ~thin_list() {
            // m_destroy()
            thin_vector< iterator > tmpVce;
            if ( 0 != m_size ) {
                node_type* ptr = m_p_sentinel->_m_p_next;
                iterator tmpIter = iterator(ptr);
                while ( tmpIter != end() ) {
                    tmpVce.push_back( tmpIter++ );
                }
                // tmpIter = iterator( ptr );
                for ( auto elem : tmpVce ) {
                    m_destroy( elem._m_p_node );
                    m_deallocate( elem._m_p_node );
                }
            }
            m_deallocate( m_p_sentinel );
            m_size = 0;
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
        void push_back( T&& elem ) {
            node_type* ptr =  m_allocate();
            m_construct( ptr, std::move( elem ) );
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
        void push_front( T&& elem ) {
            node_type* ptr = m_allocate();
            m_construct( ptr, std::move( elem ) );
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
        // 在迭代器 `posIter` 前就地构造元素，返回新元素的迭代器（C++11起）
        template < typename... Args >
        iterator emplace( const_iterator posIter, Args&&...args ) {
            //首先构造新节点
            node_type* ptr = m_allocate();
            m_construct( ptr, std::forward<Args>(args)... );

            if ( posIter._m_p_node == m_p_sentinel && 0 == m_size ) {
                ptr->_m_p_next = posIter._m_p_node;
                ptr->_m_p_prev = posIter._m_p_node;
                posIter._m_p_node->_m_p_prev = ptr;
                posIter._m_p_node->_m_p_next = ptr;
            } else {
                ptr->_m_p_next = posIter._m_p_node;
                ptr->_m_p_prev = posIter._m_p_node->_m_p_prev;
                posIter._m_p_node->_m_p_prev->_m_p_next = ptr;
                posIter._m_p_node->_m_p_prev = ptr;
            }
            ++m_size;
            return iterator(ptr);
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
        //在posIter 前插入元素，返回插入元素的迭代器
        iterator insert( const_iterator posIter, const T& elem ) {
            node_type* ptr = m_allocate();
            m_construct( ptr, elem );
            node_type* temp = posIter._m_p_node;
            if ( 0 == m_size ) {
                ptr->_m_p_next = temp;
                ptr->_m_p_prev = temp;
                temp->_m_p_prev = ptr;
                temp->_m_p_next = ptr;
            } else {
                ptr->_m_p_next = temp;
                ptr->_m_p_prev = temp->_m_p_prev;
                temp->_m_p_prev->_m_p_next = ptr;
                temp->_m_p_prev = ptr;
            }
            ++m_size;
            return iterator(ptr);
        }
        iterator insert( const_iterator posIter, T&& elem ) {
            node_type* ptr = m_allocate();
            m_construct( ptr, std::move( elem ) );
            node_type* temp = posIter._m_p_node;
            if ( 0 == m_size ) {
                ptr->_m_p_next = temp;
                ptr->_m_p_prev = temp;
                temp->_m_p_prev = ptr;
                temp->_m_p_next = ptr;
            } else {
                ptr->_m_p_next = temp;
                ptr->_m_p_prev = temp->_m_p_prev;
                temp->_m_p_prev->_m_p_next = ptr;
                temp->_m_p_prev = ptr;
            }
            ++m_size;
            return iterator(ptr);
        }
        // 插入 cnt 个 elem
        iterator insert( const_iterator posIter, size_type cnt, const T& elem ) {
            const_iterator insertPos = posIter;
            for ( size_type i = 0; i != cnt; ++i ) {
                insertPos  =  insert( insertPos, elem );
            }
            return iterator(insertPos._m_p_node);
        }
        // 在 posIter  前插入范围 [first, last) 的元素
        template < typename InputIter,
                   typename std::enable_if< _is_input_iterator< InputIter >::value>::type* = nullptr >
        iterator insert( const_iterator posIter, InputIter first, InputIter last ) {
            const_iterator insertPos = posIter;
            InputIter iter = first;
            iterator retIter = iterator( posIter._m_p_node );
            while ( iter != last ) {
                if ( iter == first ) {
                    retIter =  insert( insertPos, *( iter++ ) );
                } else {
                    insert( insertPos, *( iter++ ) );
                }
            }
            return retIter;
        }
        // 插入初始化列表中的元素（C++11起）
        iterator insert( const_iterator posIter, std::initializer_list< T > initlst ) {
            const_iterator insertPos = posIter;
            auto iter = initlst.begin();
            iterator retIter = iterator( posIter._m_p_node );
            while ( iter != initlst.end() ) {
                if ( iter == initlst.begin() ) {
                    retIter =  insert( insertPos, *( iter++ ) );
                } else {
                    insert( insertPos, *( iter++ ) );
                }
            }
            return retIter;
        }
        // 删除 posIter  处的元素，返回被删元素的下一个位置的迭代器--m_size
        iterator erase( const_iterator posIter ) {
            node_type* ptr = posIter._m_p_node;
            ptr->_m_p_prev->_m_p_next = ptr->_m_p_next;
            ptr->_m_p_next->_m_p_prev = ptr->_m_p_prev;
            --m_size;
            iterator retIter = iterator( ptr->_m_p_next );
            m_destroy( ptr );
            m_deallocate( ptr ); ptr = nullptr;
            return retIter;
        }
        // 删除 [first, last) 范围的元素，返回 last 的迭代器
        iterator erase( const_iterator first, const_iterator last ) {
            while ( first != last ) {
                erase( (first++) );
            }
            return iterator( last._m_p_node );
        }
        // 交换两个列表的内容（O(1) 时间复杂度）
        void swap( thin_list& other ) noexcept {
            node_type* temp = m_p_sentinel;
            m_p_sentinel = other.m_p_sentinel;
            other.m_p_sentinel = temp;
            size_type tmp = m_size;
            m_size = other.m_size;
            other.m_size = tmp;
        }
       // 替换列表内容为 `count` 个 `value`
        void assgin( size_type cnt, const T& value ) {
            if ( cnt <= m_size ) {
                iterator iter = begin();
                for ( size_type i = 0; i != cnt; ++i ) {
                    *(iter++) = value;
                }
                erase( iter, end() );
            } else {
                for ( iterator iter = begin(); iter != end(); ) {
                    *(iter++) = value;
                }
                while ( m_size < cnt ) {
                    push_back( value );
                }
            }
        }
        // 替换内容为迭代器范围 `[first, last)` 的元素
        template < typename InputIter, 
                   typename std::enable_if< _is_input_iterator< InputIter >::value>::type* = nullptr >
        void assgin( InputIter first, InputIter last ) {
            if ( m_size == 0 ) {
                while ( first != last ) {
                    push_back( *(first++) );
                }
            } else {
                iterator iter = begin();
                while ( first != last && iter != end() ) {
                    *(iter++) = *first++;
                }
                if ( first == last ) {
                    erase( iter, end() );
                } else {
                    while ( first != last ) {
                        push_back( *(first++) );
                    }
                }
            }
        }
        // 替换内容为初始化列表 `ilist` 中的元素（C++11起）
        void assgin( std::initializer_list< T > lst ) {
            assgin( lst.begin(), lst.end() );
        }
        // 合并两个有序列表：将 other 的元素移动到当前列表，合并后 other 为空--前提：当前列表和other必须已按相同顺序排序
        void merge( thin_list& other ) {
            if ( other.m_size == 0) {
            } else if ( m_size == 0 && other.m_size != 0 ) {
                assgin( other.begin(), other.end() );
                other.clear();
            } else if ( m_size != 0 && other.m_size != 0 ) {
                auto iter = begin();
                auto otherIter = other.begin();
                for ( ; otherIter != other.end(); ++otherIter ) {
                    for ( ; iter != end(); ++iter ) {
                        if ( *otherIter <= *iter ) {
                            insert( iter, *otherIter );
                            break;
                        } 
                    }
                    if ( iter == end() ) {
                        break;
                    }
                }
                while( otherIter != other.end() ) {
                    push_back( *(otherIter++) );
                }
                other.clear();
            }

        }
        // 使用自定义比较器 `comp` 合并两个有序列表
        template < typename Compare >
        void merge( thin_list& other, Compare comp ) {
            if ( other.m_size == 0) {
            } else if ( m_size == 0 && other.m_size != 0 ) {
                assgin( other.begin(), other.end() );
                other.clear();
            } else if ( m_size != 0 && other.m_size != 0 ) {
                auto iter = begin();
                auto otherIter = other.begin();
                for ( ; otherIter != other.end(); ++otherIter ) {
                    for ( ; iter != end(); ++iter ) {
                        if ( comp( *otherIter, *iter ) ) {  //*otherIter <= *iter
                            insert( iter, *otherIter );
                            break;
                        } 
                    }
                    if ( iter == end() ) {
                        break;
                    }
                }
                while( otherIter != other.end() ) {
                    push_back( *(otherIter++) );
                }
                other.clear();
            }
        }
        // 将 other 的所有元素移动到当前列表的 posIter 前（other 必须与当前列表不同）
        void splice( const_iterator posIter, thin_list& other ) {
            if ( other.m_size != 0 ) {
                if ( m_size != 0 ) {
                    other.m_p_sentinel->_m_p_next->_m_p_prev = posIter._m_p_node->_m_p_prev;
                    posIter._m_p_node->_m_p_prev->_m_p_next = other.m_p_sentinel->_m_p_next;
                    posIter._m_p_node->_m_p_prev = other.m_p_sentinel->_m_p_prev;
                    other.m_p_sentinel->_m_p_prev->_m_p_next = posIter._m_p_node;
                } else {
                    other.m_p_sentinel->_m_p_next->_m_p_prev = m_p_sentinel;
                    m_p_sentinel->_m_p_next = other.m_p_sentinel->_m_p_next;
                    m_p_sentinel->_m_p_prev = other.m_p_sentinel->_m_p_prev;
                    other.m_p_sentinel->_m_p_prev->_m_p_next = m_p_sentinel;
                }
                other.m_p_sentinel->_m_p_next = other.m_p_sentinel;
                other.m_p_sentinel->_m_p_prev = other.m_p_sentinel;
                m_size += other.m_size;
                other.m_size = 0;
            }
            
        }
        // 将 other 中 it 指向的元素移动到当前列表的 posIter 前
        void splice( const_iterator posIter, thin_list& other, const_iterator iter ) {
            //将other中移动节点“摘出“来
            iter._m_p_node->_m_p_prev->_m_p_next = iter._m_p_node->_m_p_next;
            iter._m_p_node->_m_p_next->_m_p_prev = iter._m_p_node->_m_p_prev;
            other.m_size -= 1;
            //放入指定位置
            if ( m_size != 0 ) {
                iter._m_p_node->_m_p_prev = posIter._m_p_node->_m_p_prev;
                iter._m_p_node->_m_p_next = posIter._m_p_node;
                posIter._m_p_node->_m_p_prev->_m_p_next = iter._m_p_node;
                posIter._m_p_node->_m_p_prev = iter._m_p_node;
            } else {
                iter._m_p_node->_m_p_next = posIter._m_p_node;
                iter._m_p_node->_m_p_prev = posIter._m_p_node;
                posIter._m_p_node->_m_p_next = iter._m_p_node;
                posIter._m_p_node->_m_p_prev = iter._m_p_node;
            }
            m_size += 1;
        }
        // 将 other 中 [first, last) 范围的元素移动到当前列表的 posIter 前
        void splice( const_iterator posIter, thin_list& other, const_iterator first, const_iterator last ) {
            size_type cnt = 0;
            auto tmp = first;
            while ( tmp != last ) {
                ++cnt; ++tmp;
            }
            if ( m_size == 0 ) {
                posIter._m_p_node->_m_p_next = first._m_p_node;
                posIter._m_p_node->_m_p_prev = last._m_p_node->_m_p_prev;
                first._m_p_node->_m_p_prev->_m_p_next = last._m_p_node;
                last._m_p_node->_m_p_prev->_m_p_next = posIter._m_p_node;
                last._m_p_node->_m_p_prev = first._m_p_node->_m_p_prev;
                first._m_p_node->_m_p_prev = posIter._m_p_node;
            } else {
                first._m_p_node->_m_p_prev->_m_p_next = last._m_p_node;
                node_type* temp = last._m_p_node->_m_p_prev;
                last._m_p_node->_m_p_prev = first._m_p_node->_m_p_prev;
                posIter._m_p_node->_m_p_prev->_m_p_next = first._m_p_node;
                first._m_p_node->_m_p_prev = posIter._m_p_node->_m_p_prev;
                temp->_m_p_next = posIter._m_p_node;
                posIter._m_p_node->_m_p_prev = temp;
            }
            m_size += cnt;
            other.m_size -= cnt;
        }
        // 删除所有值等于 `value` 的元素
        void remove(const T& value) {
            if ( m_size != 0 ) {
                for ( auto iter = begin(); iter != end(); ) {
                    if ( *iter == value ) {
                        iter =  erase( iter );
                    } else {
                        ++iter;
                    }
                }
            }
        }
        // 删除所有满足谓词 pred 的元素（`pred(element)` 返回 `true` 时删除）
        template < typename Predicate > 
        void remove_if( Predicate pred ) {
            if ( m_size != 0 ) {
                for ( auto iter = begin(); iter != end(); ) {
                    if ( pred( *iter ) ) {
                       iter =  erase( iter );
                    } else {
                        ++iter;
                    }
                }
            }
        }
        // 反转列表元素的顺序（O(n) 时间复杂度）
        void reverse() noexcept {
            iterator currIter = begin();
            size_type cnt = 0;
            while ( cnt != m_size + 1) {
                node_type* temp = currIter._m_p_node->_m_p_next;
                currIter._m_p_node->_m_p_next = currIter._m_p_node->_m_p_prev;
                currIter._m_p_node->_m_p_prev = temp;
                currIter = iterator(temp);
                ++cnt;
            }
        }
        // 删除连续重复元素（保留第一个）
        void unique() {
            T temp = *( begin() );
            for ( iterator iter = ++begin(); iter != end(); ) {
                if( temp == *iter ) {
                    iter = erase( iter );
                } else {
                    temp = *iter;
                    ++iter;
                }
            }
        }
        // 使用二元谓词 `pred` 判断是否重复（例如自定义相等条件）
        template < typename BinaryPredicate >
        void unique( BinaryPredicate pred ) {
            T temp = *( begin() );
            for ( iterator iter = ++begin(); iter != end(); ) {
                if( pred( temp, *iter ) ) { //(temp == *iter)
                    iter = erase( iter );
                } else {
                    temp = *iter;
                    ++iter;
                }
            }
        }
        // 对列表元素进行升序排序（默认使用 `<` 运算符）
        void sort() {
            
        }
        // 使用自定义比较器 `comp` 排序
        template < typename  Compare > 
        void sort( Compare comp ) {
            
        }
        // 清空列表中的所有元素
        void clear() noexcept {
            erase( begin(), end() );
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