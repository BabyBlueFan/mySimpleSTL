/*2025.02.14*/
//<thin_vector>
#ifndef __THIN_VECTOR__
#define __THIN_VECTOR__

#include <iostream>
#include "thin_vector_iterator.hpp"

namespace thinSTL {
    template< typename T >
    class thin_vector {
    public:
        using iterator = thin_vector_iterator< T >;
        using const_iterator = thin_vector_iterator< const T >;
    private:
        T* _m_start; //指向动态数组的起始位置
        T* _m_finsh; //指向当前最后一个元素的下一个位置
        T* _m_end_of_storage;//指向分配的内存末尾
    public:
        //默认构造函数
        thin_vector() noexcept : _m_start( nullptr ), _m_finsh( nullptr ), _m_end_of_storage( nullptr ) {}
        //指定大小和初始值的构造函数
        thin_vector( size_t size, const T& elem )
        {
            _m_start = new T[size];
            for ( size_t i = 0; i != size; ++i ) {
                _m_start[i] = elem;
            }
            _m_finsh = _m_start + size;
            _m_end_of_storage = _m_finsh;
        }
        //初始化列表
        thin_vector( std::initializer_list< T > init )
        {
            _m_start = new T[init.size()];
            std::copy( init.begin(), init.end(), _m_start );
            _m_finsh = _m_start + init.size();
            _m_end_of_storage = _m_finsh;
        }
        //迭代器范围的构造函数
        template< typename InputIterator >
        thin_vector( InputIterator first, InputIterator last )
        {
            size_t n = std::distance( first, last );
            _m_start = new T[n];
            _m_finsh = _m_start + n;
            _m_end_of_storage = _m_finsh;
            std::copy( first, last, _m_start );
        }
        //拷贝构造函数
        thin_vector( const thin_vector& other )
        {
            size_t n = other._m_finsh - other._m_start;
            _m_start = new T[n];
            _m_finsh = _m_start + n;
            _m_end_of_storage = _m_finsh;
            std::copy( other._m_start, other._m_finsh, _m_start );

        }
        //移动拷贝构造函数
        thin_vector( thin_vector&& other ) {
            _m_start = other._m_start;
            _m_finsh = other._m_finsh;
            _m_end_of_storage = other._m_end_of_storage;
            other._m_start = nullptr;
            other._m_finsh = nullptr;
            other._m_end_of_storage = nullptr;
        }
        //析构函数
        ~thin_vector(){ 
            if ( _m_start != nullptr ) {
                delete[] _m_start; _m_start = nullptr;
            }
            std::cout << "~" << std::endl;
        }
        //operator=
        thin_vector& operator=( const thin_vector& other ) {
            size_t n = std::distance( other._m_start, other._m_finsh );
            _m_start = new T[n];
            _m_finsh = _m_start + n;
            _m_end_of_storage = _m_finsh;
            std::copy( other._m_start, other._m_finsh, _m_start );
            return *this;
        }
        //operator=&&
        thin_vector& operator=( thin_vector&& other ) {
            _m_start = other._m_start;
            _m_finsh = other._m_finsh;
            _m_end_of_storage = other._m_end_of_storage;
            other._m_start = nullptr;
            other._m_finsh = nullptr;
            other._m_end_of_storage = nullptr;
            return *this;
        }

        //size()-->返回容器中元素的数量
        size_t size() const;

        void show_thin_vector() const {
            T* ptr = _m_start;
            for ( ; ptr != _m_finsh; ++ptr ) {
                std::cout << *ptr << " ";
            }
            std::cout << std::endl;
            return;
        }
    };

} //namespace thinSTL


#include "../source/thin_vector.cpp"

#endif //__THIN_VECTOR__