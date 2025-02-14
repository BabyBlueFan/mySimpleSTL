#ifndef __THIN_VECTOR_ITERATOR__
#define __THIN_VECTOR_ITERATOR__

#include <iostream>

namespace thinSTL {
    template< typename T >
    class thin_vector_iterator {
    private:
        T* ptr;
    public:
        //构造函数
        thin_vector_iterator( T* p = nullptr ) : ptr( p ) {}
        //拷贝构造函数
        thin_vector_iterator( const thin_vector_iterator& other ) {
            ptr = other.ptr;
        } 
        /* ********************以下为迭代器提供的操作************************* */
        //解引用操作 operator*
        T& operator*() const {
            return *ptr;
        }

        //箭头操作符operator->
        T* operator->() const {
            return ptr;
        }

        //前缀++
        thin_vector_iterator& operator++() {
            ++ptr;
            return *this;
        }
        //后缀++
        thin_vector_iterator operator++(int) {
            thin_vector_iterator temp = *this;
            ++ptr;
            return temp;
        }
        //前缀--
        thin_vector_iterator& operator--() {
            --ptr;
            return *this;
        }
        //后缀--
        thin_vector_iterator operator--(int) {
            thin_vector_iterator temp = *this;
            --ptr;
            return temp;
        }

        //加法操作符 operator+
        thin_vector_iterator operator+( std::ptrdiff_t dis ) const {
            return thin_vector_iterator( ptr + dis );
        }

        //减法操作符 operator-
        thin_vector_iterator operator-( std::ptrdiff_t dis ) const {
            return ( ptr - dis );
        }

        //差异操作符 operator-
        std::ptrdiff_t operator-( const thin_vector_iterator& other ) const {
            return ptr - other.ptr;
        }

        //赋值操作符 operator=
        thin_vector_iterator& operator=( const thin_vector_iterator& other ) {
            ptr = other.ptr;
            return *this;
        }

        //关系操作符 ==, !=, <, <=, >, >=
        //operator==
        bool operator==( const thin_vector_iterator& other ) const {
            return ptr == other.ptr;
        }
        //operator!=
        bool operator!=( const thin_vector_iterator& other ) const {
            return ptr != other.ptr;
        }
        //operator<
        bool operator<( const thin_vector_iterator& other ) const {
            return ptr < other.ptr;
        }
        //operator<=
        bool operator<=( const thin_vector_iterator& other ) const {
            return ptr <= other.ptr;
        }
        //operator>
        bool operator>( const thin_vector_iterator& other ) const {
            return ptr > other.ptr;
        }
        //operator>=
        bool operator>=( const thin_vector_iterator& other ) const {
            return ptr <= other.ptr;
        }
    };
}

#endif // __THIN_VECTOR_ITERATOR__