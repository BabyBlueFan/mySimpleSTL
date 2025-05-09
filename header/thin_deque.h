/* 2025.02.27 */
#ifndef __THIN_DEQUE_H__
#define __THIN_DEQUE_H__

#include <iostream>
#include <memory>
#include <new>
#include <initializer_list>
#include <type_traits>
#include "iterator_category_judgment.h"

namespace thinContainers {

    #define MAP_MIN_SIZE 8

    //deque的缓冲区大小的设定
    //若 n = 0; 则根据sizeof(T)来确定大小
    //否则，n
    inline size_t __deque_buf_size( size_t n, size_t sizeofT ) {
        return n != 0 ? (n) : (sizeofT < 512 ? ( sizeof( 512/sizeofT ) ) : ( size_t(1) ) );
    }

    template < typename T, size_t BufSiz = 0 >
    class __deque_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using const_reference = const T&;
        using const_pointer = const T*;
        template < typename U, typename Alloc, size_t _BufSiz > friend class thin_deque; 
        template < typename U, size_t _BufSiz > friend class __deque_iterator;
    private: //数据成员 
        pointer m_cur;  //指向当前元素
        pointer m_first; //当前元素所在缓冲区的头
        pointer m_last; //当前元素所在缓冲区的尾
        pointer* m_node; //map对应的缓冲区
    public:
        __deque_iterator() {}
        //note: 允许非const到const转换
        template < typename NON_CONST,
        typename =  typename std::enable_if< std::is_convertible< NON_CONST*, T*>::value >::type >
        __deque_iterator( const __deque_iterator< NON_CONST, BufSiz >& other )
        : m_cur( other.m_cur), m_first( other.m_first ),m_last( other.m_last ), 
        m_node( const_cast<typename __deque_iterator<T, BufSiz>::pointer*>(other.m_node) )
        {}
        static size_t buffer_size() {
            return __deque_buf_size( BufSiz, sizeof(T) );
        }
        //operator*
        T& operator*() {
            return *m_cur;
        }
        const T& operator*() const {
            return *m_cur;
        }
        T*  operator->() {
            return m_cur;
        }
        const T* operator->() const {
            return m_cur;
        }
        //operator-
        difference_type operator-( const __deque_iterator& other ) const {
            difference_type disPtr = 0;
            if ( m_node == other.m_node ) {
                disPtr = m_cur - other.m_cur;
            } else {
                disPtr = difference_type(buffer_size()) * ( m_node - other.m_node -1 ) + 
                        ( m_cur - m_first ) + ( other.m_last - other.m_cur );
            }
            return disPtr;
        }
        //operator++ 前置
        __deque_iterator& operator++() {
            ++m_cur;
            if ( m_cur == m_last ) {
                m_set_node( m_node + 1 );
                m_cur = m_first;
            }
            return *this;
        }
        //operator++ 后置
        __deque_iterator operator++( int ) {
            __deque_iterator temp = *this;
            ++*this;
            return temp;
        }
        //operator-- 前置
        __deque_iterator& operator--() {
            if ( m_cur == m_first ) {
                m_set_node( m_node - 1 );
                m_cur = m_last - 1;
            } else {
                --m_cur;
            }
            return *this;
        }
        //operator-- 后置
        __deque_iterator operator--(int) {
            __deque_iterator temp = *this;
            --*this;
            return temp;
        }
        //operator+=
        __deque_iterator& operator+=( difference_type n ) {
            if ( n >= 0 ) {
                if ( n < ( m_last - m_cur ) ) {
                    m_cur += n;
                } else {
                    difference_type off_node = ( n - ( m_last - m_cur  ) ) / difference_type(buffer_size());
                    difference_type off_elem = ( n - ( m_last - m_cur  ) ) % difference_type(buffer_size());
                    m_set_node( m_node + off_node + 1 );
                    m_cur = m_first + off_elem;
                }
            } else {
                difference_type m = -n;
                if ( m <= ( m_cur - m_first ) ) {
                    m_cur -= m;
                } else {
                    difference_type off_node = ( m - ( m_cur - m_first ) ) / difference_type(buffer_size());
                    difference_type off_elem = ( m - ( m_cur - m_first ) ) % difference_type(buffer_size());
                    m_set_node( m_node - off_node -1 );
                    m_cur = m_last - off_elem;
                }
            }
            return *this;
        } 
        //operator+
        __deque_iterator operator+( difference_type n ) const {
            __deque_iterator temp = *this;
            return temp += n;
        }
        //operator-=
        __deque_iterator& operator-=( difference_type n ) {
            return *this += -n;
        }
        //operator-
        __deque_iterator operator-( difference_type n ) const {
            __deque_iterator temp = *this;
            return temp -= n;
        }
        //operator[]
        reference operator[]( difference_type n ) const {
            return *( *this + n );
        }
        //operator==
        bool operator==( const __deque_iterator& other ) const {
            return m_cur == other.m_cur;
        }
        //operator!=
        bool operator!=( const __deque_iterator& other ) const {
            return !(*this == other);
        }
        //operator<
        bool operator<( const __deque_iterator& other ) const {
            return (m_node == other.m_node) ? ( m_cur < other.m_cur ) : ( m_node < other.m_node );
        }
        //operator=
        __deque_iterator& operator=( const __deque_iterator& other ) {
            m_cur = other.m_cur;
            m_first = other.m_first;
            m_last = other.m_last;
            m_node = other.m_node;
            return *this;
        }
    protected:
        inline void m_set_node( pointer* new_node ) {
            m_node = new_node;
            m_first = *new_node;
            m_last = m_first + difference_type( buffer_size() );
        }
    };  //__deque_iterator

    template < typename T, typename Alloc = std::allocator< T >, size_t BufSiz = 0  >
    class thin_deque {
    public:
        using iterator = __deque_iterator< T, BufSiz >;
        using const_iterator = __deque_iterator< const T, BufSiz >;
        // using const_iterator = __list_const_iterator< T >;
        // using reverse_iterator = __list_reverse_iterator< T >;
        // using const_reverse_iterator = __list_const_reverse_iterator< T >;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using allocator_type = Alloc;
        allocator_type get_allocator() const noexcept {
            return allocator_type();
        }

    private: //数据成员
        iterator m_beg;
        iterator m_end;
        pointer* m_map;
        size_type m_map_size;

    public:
        //默认构造函数
        thin_deque() {}
        thin_deque( size_type n, const T& value ) {
            // m_create_map_and_nodes( n );
            std::cout << "bufferSize = " << m_get_buffer_size() << std::endl;
            m_fill_initialize( n, value );
        }
        //析构函数
        ~thin_deque() {
            //析构元素对象
            for ( iterator iter = m_beg; iter != m_end; ++iter ) {
                m_destroy( iter.m_cur );
            }
            //释放缓冲区
            iterator iter = m_beg;
            while ( (m_end - iter) >= 0  ) {
                pointer temp = iter.m_first;
                m_deallocate( temp, m_get_buffer_size() );
                iter += m_get_buffer_size();
            }
            //析构中控区
            // delete [] m_map;
            std::allocator< pointer >().deallocate( m_map, m_map_size );
        }
        //begin()
        iterator begin() {
            return m_beg;
        }
        const_iterator begin() const {
            return m_beg;
        }
        //end()
        iterator end() {
            return m_end;
        }
        //[]
        reference operator[]( size_type n ) {
            return m_beg[difference_type(n)];
        } 
        //front()
        reference front() {
            return *m_beg;
        }
        //back()
        reference back() {
            iterator temp = m_end;
            --temp;
            return *temp;
        }
        size_type size() const {
            return ( m_end - m_beg );
        }
        bool empty() const {
            return m_beg == m_end;
        }
        //push_back()
        void push_back( const T& value ) {
            // ++m_end.m_cur;
            pointer temp = m_end.m_cur;
            if ( ++temp == m_end.m_last ) {
                m_construct( m_end.m_cur, value );
                //**一定条件下换中控区
                if ( m_map + m_map_size - 1 == m_end.m_node ) {
                    __reserve_map_at_back();
                } 
                pointer new_buffer =  m_allocate( m_get_buffer_size() );
                *(m_end.m_node + 1) = new_buffer;
                m_end.m_set_node( m_end.m_node + 1);
                m_end.m_cur = m_end.m_first;
            } else {
                m_construct( m_end.m_cur, value );
                ++m_end;
            }
        }
        //push_front()
        void push_front( const T& value ) {
            if ( m_beg.m_cur == m_beg.m_first ) {
                if ( m_beg.m_node ==  m_map ) { //需要分配空间
                    __reserve_map_at_front();
                }
                pointer new_buffer = m_allocate( m_get_buffer_size() );
                *(m_beg.m_node - 1) = new_buffer;
                m_beg.m_set_node( m_beg.m_node - 1 );
                m_beg.m_cur = m_beg.m_last - 1; 
                m_construct( m_beg.m_cur, value );
            } else {
                m_construct( --m_beg.m_cur, value );
            }
        }
        //pop_back()
        void pop_back() {
            if ( m_beg == m_end ) {
                return;
            }
            if ( m_end.m_cur == m_end.m_first ) {
                pointer* mapNode = m_end.m_node;
                //迭代器m_end换区了-->需要释放缓冲区
                m_destroy( (--m_end).m_cur );
                m_deallocate( *(mapNode), m_get_buffer_size() );
            } else {
                m_destroy( (--m_end).m_cur );
            }
        }
        //pop_front()
        void pop_front() {
            if ( m_beg == m_end ) {
                return;
            }
            if ( m_beg.m_cur == m_beg.m_last - 1 ) {
                m_destroy( m_beg.m_cur );
                pointer ptr = *(m_beg.m_node);
                ++m_beg;
                m_deallocate( ptr, m_get_buffer_size() );
            } else {
                m_destroy( m_beg.m_cur );
                ++m_beg;
            }
        }
        //clear()用来清除整个deque，回归到最初状态（无任何元素）。但是`保留一个缓冲区` --保留m_beg所在的缓冲区
        //m_beg 和  m_end 中间的缓冲区必然是饱和的。
        void clear() {
            if ( m_beg == m_end ) {
                return;
            }
            if ( m_beg.m_node == m_end.m_node ) { //m_beg 和 m_end在一个缓冲区中
                for ( ; m_beg != m_end; ++m_beg ) {
                    m_destroy( m_beg.m_cur );
                }
            } else {
                for ( pointer* map_ptr = m_beg.m_node + 1; map_ptr != m_end.m_node; ++map_ptr ) {
                    //首先析构元素
                    for ( size_type cnt = 0; cnt != m_get_buffer_size(); ++cnt ) {
                        m_destroy( *(map_ptr) + cnt );
                    }
                    //释放缓冲区
                    m_deallocate( *(map_ptr), m_get_buffer_size() );
                }
                //析构m_end所在的缓冲区中的元素
                for ( pointer elem_ptr = m_end.m_first; elem_ptr != m_end.m_cur; ++elem_ptr ) {
                    m_destroy( elem_ptr );
                }
                //释放m_end所在的缓冲区
                m_deallocate( *(m_end.m_node), m_get_buffer_size() );
                //析构m_beg所在的缓冲区中的元素
                for ( pointer elem_ptr = m_beg.m_cur; elem_ptr != m_beg.m_last; ++elem_ptr ) {
                    m_destroy( elem_ptr );
                }
                //最后使m_beg 和 m_end 指向同一个区域
                m_end = m_beg;
            }
        }
        // erase()
        //erase( iterator posIter ) :清除posIter所指的元素
        iterator erase( iterator posIter ) {
            iterator nextIter = posIter + 1;
            difference_type dis = posIter - m_beg;
            //判断删除点之前和之后的元素个数，选择较少的区段进行移动
            if ( dis < difference_type( size() >> 1 ) ) {
                std::copy_backward( m_beg, posIter, nextIter );
                pop_front();
            } else {
                // iterator ii = m_end - 1;
                std::copy( nextIter, m_end, posIter );
                pop_back();
            }
            return posIter;
        }
        iterator erase( iterator first, iterator last ) {
            if ( first == m_beg && last == m_end ) {
                clear();
                return m_end;
            }
            difference_type fir_beg_dis = first - m_beg;
            difference_type end_last_dis = m_end -last;
            difference_type dis = last - first;
            iterator retIter = m_end;
            if ( fir_beg_dis < end_last_dis ) {
                std::copy_backward( m_beg, first, last );
                /* for ( difference_type cnt = 0; cnt != dis; ++cnt ) {
                    m_destroy( m_beg.m_cur + cnt );
                }
                // iterator temp = m_beg;
                pointer* map_node = m_beg.m_node;
                m_beg += dis; 
                for ( ; map_node != m_beg.m_node; ++map_node ) {
                    m_deallocate( *(map_node), m_get_buffer_size() );
                } */
                for ( difference_type cnt = 0; cnt != dis; ++cnt ) {
                    pop_front();
                }
                retIter = last;
            } else {
                std::copy( last, m_end, first );
                for ( difference_type cnt = 0; cnt != dis; ++cnt ) {
                    pop_back();
                }
                retIter = first;
            }
            return retIter;
        }
        //insert
        iterator insert( iterator posIter, const T& value ) {
            if ( posIter == m_beg ) {
                push_front( value );
            } else if (posIter == m_end ) {
                push_back( value );
            } else {
                difference_type dis_1 = posIter - m_beg;
                difference_type dis_2 = m_end - posIter;
                if ( dis_1 < dis_2 ) {
                    push_front( T() );
                    posIter = m_beg + dis_1 + 1;
                    std::copy( m_beg + 1, posIter, m_beg );
                    *(--posIter) = value;
                } else {
                    push_back( T() );
                    posIter = m_end - dis_2 - 1;
                    std::copy_backward( posIter, m_end - 1, m_end );
                    *posIter = value;
                }
            }
            return posIter;
        }
    protected:
        //缓冲区的大小计算
        static size_t m_get_buffer_size() {
            return  __deque_buf_size( BufSiz, sizeof(T) );
        } 
        //申请map空间
        pointer* m_map_allocate( size_type n ) {
            return std::allocator< pointer >().allocate( n );
        }
        void __reserve_map_at_back( size_type node_to_add = 1 ) {
            pointer* tmp =  m_map_allocate( m_map_size + node_to_add ); //分配新的map内存空间
            std::uninitialized_copy_n( m_map, m_map_size, tmp ); //将旧的map内存空间拷贝之新的map内存空间
            m_beg.m_set_node( (m_beg.m_node - m_map) + tmp );
            m_beg.m_cur = m_beg.m_first;
            m_end.m_set_node( (m_end.m_node - m_map) + tmp );
            m_end.m_cur = m_end.m_last - 1;
            std::allocator< T* >().deallocate( m_map, m_map_size  );
            m_map = tmp;
            m_map_size = m_map_size + node_to_add;
        }
        void __reserve_map_at_front( size_type node_to_add = 1 ) {
            pointer* tmp = m_map_allocate( m_map_size + node_to_add ); //分配新的map内存空间
            std::uninitialized_copy_n( m_map, m_map_size, tmp + node_to_add ); //将旧的map内存空间拷贝之新的map内存空间
            m_beg.m_set_node( tmp + node_to_add );
            m_beg.m_cur = m_beg.m_first;
            difference_type offset = m_end.m_last - m_end.m_cur;
            m_end.m_set_node( tmp + node_to_add + (m_end.m_node - m_map) );
            m_end.m_cur = m_end.m_last - offset;
            std::allocator< T* >().deallocate( m_map, m_map_size  );
            m_map = tmp;
            m_map_size = m_map_size + node_to_add;
        }
        //deque的内存空间的设计--中控器、缓冲区、m_beg m_end m_map m_map_size
        void m_create_map_and_nodes( size_type elem_num ) {
            size_type node_num = elem_num / m_get_buffer_size() + 1;
            m_map_size = MAP_MIN_SIZE > ( node_num + 2 ) ? MAP_MIN_SIZE : (node_num + 2);
            m_map =  m_map_allocate( m_map_size );
            pointer* nstart = m_map + ( m_map_size - node_num ) / 2; //已使用第一块缓冲区的地址
            pointer* nfinish = nstart + node_num - 1; //已使用最后一块缓冲区的地址
            //配置缓冲区
            for ( pointer* cur = nstart; cur <= nfinish; ++cur ) {
                *cur = m_allocate( m_get_buffer_size() ); 
            }
            //设置deque的数据成员m_beg m_end
            m_beg.m_set_node( nstart );
            m_beg.m_cur = *nstart;
        
            m_end.m_set_node( nfinish );
            m_end.m_cur = elem_num % m_get_buffer_size() + m_end.m_first;
        }
        //
        void m_fill_initialize( size_type n, const T& value ) {
            m_create_map_and_nodes( n );
            std::uninitialized_fill_n( m_beg, n, value );
        }

    /* ****************申请释放构建和构造析构对象******************** */
        pointer m_allocate( size_t n ) {
            return get_allocator().allocate( n );
        }
        void m_deallocate( pointer ptr, size_t n ) {
            get_allocator().deallocate( ptr, n );
        }
        template < typename... Args >
        void m_construct( pointer ptr, Args&&... args ) {
            get_allocator().construct( ptr, std::forward< Args >(args)... );
        }
        void m_destroy( pointer ptr ) {
            get_allocator().destroy( ptr );
        }

    }; //thin_deque

}   //thinContainers

#endif //__THIN_DEQUE_H__