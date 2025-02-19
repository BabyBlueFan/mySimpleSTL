/* 2025.02.19 */
#ifndef __THIN_VECTOR_H__
#define __THIN_VECTOR_H__

#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>

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
        using data_allocator = Alloc;
        data_allocator get_allocator() {
            return data_allocator();
        }

    protected:
        iterator m_start;
        iterator m_finish;
        iterator m_end_of_storage;

    public:
        //默认构造函数
        thin_vector() : m_start( nullptr ), m_finish( nullptr ), m_end_of_storage( nullptr ) {}
        //拷贝构造函数
        thin_vector( const thin_vector& other ) {
            m_start =  get_allocator().allocate( other.size() );
            // m_start = static_cast< iterator >( ::operator new( other.size() * sizeof( value_type ) ) );
            std::uninitialized_copy( other.m_start, other.m_finish, m_start );
            m_finish = m_start + other.size();
            m_end_of_storage = m_finish;
        }
        //初始化列表构造函数
        thin_vector( std::initializer_list< T > initlist ) {
            m_start = get_allocator().allocate( initlist.size() );
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
            destroyObj();
            get_allocator().deallocate( m_start, size() );
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

    private:
    void destroyObj() {
        iterator current = m_finish;
        while ( current != m_end_of_storage ) {
            get_allocator().destroy( current++ );
        }
    }


    }; //thin_vector


}


#endif //__THIN_VECTOR_H__