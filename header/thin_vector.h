/* 2025.02.19 */
#ifndef __THIN_VECTOR_H__
#define __THIN_VECTOR_H__

#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>

namespace thinContainers {
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

    protected:
        iterator m_start;
        iterator m_finish;
        iterator m_end_of_storage;

    public:
        //默认构造函数
        thin_vector() : m_start( nullptr ), m_finish( nullptr ), m_end_of_storage( nullptr ) {}
        //填充构造函数
        thin_vector( size_type n, const T& value = T() ) {
            m_start =  m_get_allocator().allocate( n );
            std::uninitialized_fill_n( m_start, n, value );
            m_finish = m_start + n;
            m_end_of_storage = m_finish;
        }
        //范围构造函数
        #if 1
        template < typename InputIter, typename = std::enable_if_t< !std::is_integral_v< InputIter > > >
        thin_vector( InputIter first, InputIter last ) {
            //m_start = m_get_allocator().allocate( last - first );
            difference_type n = std::distance( first, last );
            m_start = m_get_allocator().allocate( n );
            std::uninitialized_copy( first, last, m_start );
            m_finish = m_start + n;
            m_end_of_storage = m_finish;
        }
        #endif
        //拷贝构造函数
        thin_vector( const thin_vector& other ) {
            m_start =  m_get_allocator().allocate( other.size() ); //分配未初始化的内存空间
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
            m_start = m_get_allocator().allocate( initlist.size() );//分配未初始化的内存空间
        // m_start = static_cast< iterator >( ::operator new( initlist.size() * sizeof( value_type) ) );
            std::uninitialized_copy( initlist.begin(), initlist.end(), m_start );
            m_finish = m_start + initlist.size();
            m_end_of_storage = m_finish;
        }


        //赋值操作符operator=
        thin_vector& operator=( const thin_vector& other ) {

            return *this;
        }
        //析构函数
        ~thin_vector() {
            // std::destroy( m_finish, m_end_of_storage );
            m_destroy();//析构对象
            m_get_allocator().deallocate( m_start, size() );//释放内存空间
            // ::operator delete( m_start );
            m_start = nullptr; m_finish = nullptr; m_end_of_storage = nullptr;
        }

        //size()返回元素的个数
        size_type size() const {
            return ( m_finish - m_start );
        }
        //begin()
        iterator begin() {
            return m_start;
        }
        //end()
        iterator end() {
            return m_finish;
        }

    protected:
    inline void m_destroy() {
        iterator current = m_finish;
        while ( current != m_end_of_storage ) {
            m_get_allocator().destroy( current++ );
        }
    }
    inline allocator_type m_get_allocator() {
        return allocator_type();
    }

    }; //thin_vector


}


#endif //__THIN_VECTOR_H__