/* 2025.02.19 */
#ifndef __THIN_VECTOR_H__
#define __THIN_VECTOR_H__

#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>
#include "iterator_category_judgment.h"

namespace thinContainers {
    #define SCALEOUT 2
   /*  // 判断是否是输入迭代器
    template< typename T, typename = void >
    struct _is_input_iterator : std::false_type {};
    template < typename T >
    struct _is_input_iterator< T, 
                        std::void_t< typename std::iterator_traits< T >::iterator_category::input_iterator_tag > 
                        >: std::true_type {}; */

    template < typename T, typename Alloc = std::allocator< T >  >
    class thin_vector {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = pointer;
        using const_iterator = const T*;
        using allocator_type = Alloc;

        allocator_type get_allocator() const {
            return allocator_type();
        }

    protected:
        iterator m_start;
        iterator m_finish;
        iterator m_end_of_storage;

    public:
        //默认构造函数
        thin_vector() : m_start( nullptr ), m_finish( nullptr ), m_end_of_storage( nullptr ) {}
        //填充构造函数
        thin_vector( size_type n, const T& value = T() ) {
            m_start =  get_allocator().allocate( n );
            std::uninitialized_fill_n( m_start, n, value );
            m_finish = m_start + n;
            m_end_of_storage = m_finish;
        }
        //范围构造函数
        template < typename InputIter,
                   typename std::enable_if< _is_input_iterator< InputIter >::value>::type* = nullptr > 
        thin_vector( InputIter first, InputIter last ) {
            //m_start = get_allocator().allocate( last - first );
            difference_type n = std::distance( first, last );
            m_start = get_allocator().allocate( n );
            std::uninitialized_copy( first, last, m_start );
            m_finish = m_start + n;
            m_end_of_storage = m_finish;
        }
        //拷贝构造函数
        thin_vector( const thin_vector& other ) {
            m_start =  get_allocator().allocate( other.size() ); //分配未初始化的内存空间
            // m_start = static_cast< iterator >( ::operator new( other.size() * sizeof( value_type ) ) );
            std::uninitialized_copy( other.m_start, other.m_finish, m_start );
            m_finish = m_start + other.size();
            m_end_of_storage = m_finish;
        }
        //移动构造函数
        thin_vector( thin_vector&& other ) {
            m_start = other.m_start;
            m_finish = other.m_finish;
            m_end_of_storage = other.m_end_of_storage;
            other.m_start = nullptr; other.m_finish = nullptr; other.m_end_of_storage = nullptr;
        }
        //初始化列表构造函数
        thin_vector( std::initializer_list< T > initlist ) {
            m_start = get_allocator().allocate( initlist.size() );//分配未初始化的内存空间
        // m_start = static_cast< iterator >( ::operator new( initlist.size() * sizeof( value_type) ) );
            std::uninitialized_copy( initlist.begin(), initlist.end(), m_start );
            m_finish = m_start + initlist.size();
            m_end_of_storage = m_finish;
        }

        //析构函数
        ~thin_vector() {
            // std::destroy( m_finish, m_end_of_storage );
            m_destroy();//析构对象
            get_allocator().deallocate( m_start, capacity() );//释放内存空间
            // ::operator delete( m_start );
            m_start = nullptr; m_finish = nullptr; m_end_of_storage = nullptr;
        }

        //拷贝赋值
        thin_vector& operator=(  const thin_vector& other ) {
            while ( capacity() < other.size() ) {
                m_spaceToBig( false );
            }
            // iterator current = other.begin();
            std::uninitialized_copy_n( other.begin(), other.size(), m_start );
            m_finish = m_start + other.size();
            return *this;
        }
        //移动赋值
        thin_vector& operator=( thin_vector&& other ) {
            m_destroy_delete( m_start, size(), capacity() );
            m_start = other.begin();
            m_finish = other.end();
            m_end_of_storage = other.m_end_of_storage;
            other.m_start = nullptr;other.m_finish = nullptr; other.m_end_of_storage = nullptr;
            return *this;
        }
        //初始化列表赋值
        thin_vector& operator=(const  std::initializer_list< T >& lst ) {
            while ( capacity() < lst.size() ) {
                m_spaceToBig( false );
            }
            std::uninitialized_copy_n( lst.begin(), lst.size(), m_start );
            m_finish = m_start + lst.size();
            return *this;
        }

        //size()返回元素的个数
        size_type size() const {
            return ( m_finish - m_start );
        }
        //begin()
        iterator begin() {
            return m_start;
        }
        const_iterator begin() const {
            return m_start;
        }
        const_iterator cbegin() const {
            return m_start;
        }
        //end()
        iterator end() {
            return m_finish;
        }
        const_iterator end() const {
            return m_finish;
        }
        const_iterator cend() const {
            return m_finish;
        }
        //capacity()--容量
        size_type capacity() const {
            return m_end_of_storage - m_start;
        }
        //[]
        reference operator[]( size_type pos ) {
            return  *(m_start+pos);    
        }
        const_reference operator[]( size_type pos ) const {
            return *(m_start+pos);
        }
        //at()--边界检查
        reference at( size_type pos ) {
            if ( pos >= size() ) {
                std::cerr << "out of range!\n";
                exit(1);
            }
            return *(m_start+pos);
        }
        const_reference at( size_type pos ) const {
            if ( pos >= size() ) {
                std::cerr << "out of range!\n";
                exit(1);
            }
            return *(m_start+pos);
        }
        //front()--返回首元素引用
        reference front() {
            return *m_start;
        }
        const_reference front() const {
            return *m_start;
        }
        //back()--尾元素
        reference back() {
            return *(m_finish-1);
        }
        const_reference back() const {
            return *(m_finish-1);
        }
        //底层数组指针
        T* data() noexcept {
            return m_start;
        }
        const T* data() const noexcept {
            return m_start;
        }
        //判断是否为空 empty()
        bool empty() const {
            return size();
        }
        //预分配空间
        void reserve( size_type new_cap ) {
            _m_spaceToBig( new_cap );
        }
        //shrink_to_fit()--容量到合适大小//C++11
        void shrink_to_fit() {
            if ( size() == capacity() ) {
                return;
            }
            thin_vector( *this ).swap( *this );
        }
        //max_size()
        size_type max_size() const noexcept {
            return ( std::allocator_traits< allocator_type >::max_size( get_allocator() ) );
        }
        //swap() 两个容器交换
        void swap( thin_vector& other ) {
            iterator temp = m_start; m_start = other.m_start; other.m_start = temp;
            temp = m_finish; m_finish = other.m_finish; other.m_finish = temp;
            temp = m_end_of_storage; m_end_of_storage = other.m_end_of_storage; other.m_end_of_storage = temp;
        }
        //clear()--清空容器但是不会改变容器的容量
        void clear() {
            m_destroy_delete( m_start, size() );
            m_finish = m_start;
        }
        //push_back()
        void push_back(const T& value) {
            if ( size() == capacity() ) {
                m_spaceToBig();
            }
            new (m_finish) T(value);
            ++m_finish;
        }
        void push_back(T&& value) {
            if ( size() == capacity() ) {
                m_spaceToBig();
            }
            new (m_finish) T(value);
            // *m_finish = std::move( value );
            get_allocator().destroy( &value );
            ++m_finish;
        }
        //pop_back() 
        void pop_back() {
            get_allocator().destroy( --m_finish );
        }
        //resise()
        void resize(size_type new_size) {
            if ( new_size > size() ) {
                if ( new_size > capacity() ) {
                    _m_spaceToBig( new_size );
                }
                size_type cnt = new_size - size();
                for ( size_type i = 0; i != cnt; ++i ) {
                    new ( m_finish + i ) T();
                }
                m_finish = m_finish + cnt;

            } else {
                size_type cnt = size() - new_size;
                for ( size_type i = 0; i != cnt; ++i ) {
                    get_allocator().destroy( m_finish - 1 - i );
                }
                m_finish = m_finish - cnt;
            }
        }
        void resize(size_type new_size, const T& value) {
            if ( new_size > size() ) {
                if ( new_size > capacity() ) {
                    _m_spaceToBig( new_size );
                }
                size_type cnt = new_size - size();
                for ( size_type i = 0; i != cnt; ++i ) {
                    new ( m_finish + i ) T( value );
                }
                m_finish = m_finish + cnt;

            } else {
                size_type cnt = size() - new_size;
                for ( size_type i = 0; i != cnt; ++i ) {
                    get_allocator().destroy( m_finish - 1 - i );
                }
                m_finish = m_finish - cnt;
            }
        }
        //插入insert操作
        iterator insert(const_iterator posIter, const T& value) {
            if ( posIter > m_finish || posIter < m_start ) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            difference_type  offset = posIter - m_start;
            if ( size() == capacity() ) {
                m_spaceToBig();
            }
            new (m_finish) T();
            std::copy_backward( m_start + offset, m_finish, m_finish+1 );
            *(m_start+offset) = value;
            ++m_finish;
            return m_start + offset;
        }
        iterator insert(const_iterator posIter, T&& value) {
            if ( posIter > m_finish || posIter < m_start ) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            difference_type  offset = posIter - m_start;
            if ( size() == capacity() ) {
                m_spaceToBig();
            }
            new (m_finish) T();
            std::copy_backward( m_start + offset, m_finish, m_finish+1 );
            // *(m_start+offset) = std::move( value );
            get_allocator().destroy( &value );
            ++m_finish;
            return m_start + offset;
        }
        iterator insert(const_iterator posIter, size_type count, const T& value) {
            if ( posIter > m_finish || posIter < m_start) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            difference_type offset = posIter - m_start;
            /* while ( ( size() + count ) > capacity() ) {
                m_spaceToBig();
            } */
            if ( ( size() + count ) > capacity() ) {
                _m_spaceToBig( size() + count  );
            }
            for ( size_type i = 0; i != count; ++i ) {
                new ( m_finish+i ) T();
            }
            std::copy_backward( m_start+offset, m_finish, m_finish + count );
            for ( size_type i = 0; i != count; ++i ) {
                *(m_start+offset+i) = value;
            }
            m_finish = m_finish + count;
            return m_start +offset;
        }
        //hack:判断是否是输入迭代器
        template < typename  InputIter, 
                   typename std::enable_if< _is_input_iterator< InputIter >::value>::type* = nullptr >    
        iterator insert( const_iterator posIter, InputIter first, InputIter last ) {
            if ( posIter > m_finish || posIter < m_start  ) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            size_type cnt = std::distance( first, last );
            difference_type offset = posIter - m_start;
            if ( size() + cnt > capacity() ) {
                _m_spaceToBig( size() + cnt );
            } 
            for ( size_type i = 0; i != cnt; ++i ) {
                new (m_finish+i) T();
            }
            std::copy_backward( m_start+offset, m_finish, m_finish+cnt );
            std::copy_backward( first, last, m_start+offset+cnt );
            m_finish = m_finish + cnt;
            return m_start +offset;
        }
        iterator insert(const_iterator posIter, const std::initializer_list<T>& ilist) {
            if ( posIter > m_finish || posIter < m_start ) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            difference_type offset = posIter - m_start;
            size_type cnt = ilist.size();
            if ( size() + cnt > capacity() ) {
                _m_spaceToBig( size() + cnt );
            } 
            for ( size_type i = 0; i != cnt; ++i ) {
                new (m_finish+i) T();
            }
            std::copy_backward( m_start+offset, m_finish, m_finish+cnt );
            std::copy_backward( ilist.begin(), ilist.end(), m_start+offset+cnt );
            m_finish = m_finish + cnt;
            return m_start +offset;

        }
        template <class... Args> 
        void emplace_back(Args&&... args) {
            if ( size() == capacity() ) {
                m_spaceToBig();
            }
            new ( m_finish ) T(std::forward<Args>(args)...);
            m_finish += 1;
        }
        //删除函数
        iterator erase( const_iterator posIter ) {
            if ( posIter >= m_finish || posIter < m_start ) {
                std::cerr << "out_of_range\n ";
                exit(1);
            }
            std::copy_backward( iterator(posIter+1), m_finish, m_finish-1 );
            get_allocator().destroy(m_finish-1);
            m_finish = m_finish - 1;
            return iterator(posIter);
        }
        iterator erase(const_iterator first, const_iterator last) {
            if ( (last <= first) || (first >= m_finish || first < m_start)
                    || (last > m_finish || last < m_start) ) {
                    std::cerr << "out_of_range\n ";
                    exit(1);
            }
            size_type cnt = last -first;
            // std::copy_backward( last, m_finish, first + ( m_finish - last ) );
            std::copy_backward( (iterator)last, m_finish, m_finish - cnt );
            for ( size_type i = 0; i != cnt; ++i ) {
                get_allocator().destroy( m_finish-1 -i );
            }
            m_finish = m_finish - cnt;
            return iterator(first);
        }

    protected:
        inline void m_destroy() {
            iterator current = m_start;
            while ( current != m_finish ) {
                get_allocator().destroy( current++ );
            }
        }
        inline void m_destroy( iterator first, iterator last ) {
            while ( first != last ) {
                get_allocator().destroy( first++ );
            }
        }
        inline void m_destroy_delete( iterator _start, size_type _end_destroy, size_type _end_delete = 0 ) {
            iterator current = _start;
            //析构对象
            while ( _end_destroy != 0 ) {
                get_allocator().destroy( current++ );
                --_end_destroy;
            }
            //释放原空间
            if ( 0 != _end_delete ) {
                get_allocator().deallocate( _start, _end_delete );
            }
        }

        //空间扩容
        inline void m_spaceToBig( bool is_copy = true ) {
            if ( capacity() == 0 ) {
                m_start = get_allocator().allocate( 1 );
                m_finish = m_start;
                m_end_of_storage = m_start + 1;
            } else {
                if ( is_copy ) {
                    iterator tmp = m_start;
                    size_type n = size();
                    size_type cap = capacity();
                    m_start = get_allocator().allocate( SCALEOUT * cap );
                    std::uninitialized_copy_n( tmp, n, m_start );
                    m_finish = m_start + n;
                    m_end_of_storage = m_start + SCALEOUT * cap;
                    m_destroy_delete( tmp, n, cap );
                } else {
                    size_type cap = capacity();
                    m_destroy_delete( m_start, size(), capacity() );
                    m_start = get_allocator().allocate( SCALEOUT * cap );
                    m_finish = m_start;
                    m_end_of_storage = m_finish + SCALEOUT * cap;
                }
            }
        }
        inline void _m_spaceToBig( size_type new_cap, bool is_copy = true ) {
            if ( is_copy ) {
                iterator tmp = m_start;
                size_type n = size();
                size_type cap = capacity();
                m_start = get_allocator().allocate( new_cap );
                std::uninitialized_copy_n( tmp, n, m_start );
                m_finish = m_start + n;
                m_end_of_storage = m_start + new_cap;
                m_destroy_delete( tmp, n, cap );
            } else {
                // size_type cap = capacity();
                m_destroy_delete( m_start, size(), capacity() );
                m_start = get_allocator().allocate( new_cap );
                m_finish = m_start;
                m_end_of_storage = m_finish + new_cap;
            }
        }

    }; //thin_vector


}  //thinContainers
#endif //__THIN_VECTOR_H__