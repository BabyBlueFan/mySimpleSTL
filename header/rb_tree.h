#ifndef __RB_TREE_H__
#define  __RB_TREE_H__

#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <new>
#include <type_traits>
#include "./iterator_category_judgment.h"

namespace thinContainers {
    using __rb_tree_color_type = bool;
    const __rb_tree_color_type __rb_tree_red = false; //红色节点为0
    const __rb_tree_color_type __rb_tree_black = true; //黑色节点为1

    //红黑树的节点设计--基节点
    struct __rb_tree_node_base {
        using color_type = __rb_tree_color_type; //bool
        using base_ptr = __rb_tree_node_base*;

        //节点的数据成员
        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr mininum( base_ptr x ) {
            while ( x->left != nullptr ) {
                x = x->left;
            }
            return x;
        }

        static base_ptr maxinum( base_ptr x ) {
            while ( x->right != nullptr ) {
                x = x->right;
            }
            return x;
        }
    }; //__rb_tree_node_base

    //带有数据域的节点--真正的节点
    template < typename Value >
    struct __rb_tree_node : public __rb_tree_node_base {
        using link_type = __rb_tree_node*;
        Value value_field;
    }; // __rb_tree_node

    //基类 迭代器
    struct __rb_tree_iterator_base {
        using base_ptr = __rb_tree_node_base::base_ptr;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;

        //数据成员
        base_ptr node; //__rb_tree_node_base* node

        void increment() {
            //中序遍历：左-->根-->右，如果一个节点有右子节点，那个其后继节点一定是右子树中最小的那个。
            if ( node->right != nullptr ) { 
                node = node->right;
                while ( node->left != nullptr ) {
                    node = node->left;
                }
            } else {
                base_ptr y = node->parent;
                while ( y->right == node ) {
                    node = y;
                    y = node->parent;
                }
                if ( node->right != y ) {
                    node = y;
                }
            }
        }

        void decrement() {
            if ( node->color == __rb_tree_red && node->parent->parent == node ) {
                node = node->right;
            } else if ( node->left != nullptr ) {
                base_ptr y = node->left;
                while ( y->right != nullptr ) {
                    y = y->right;
                }
                node = y;
            } else {
                base_ptr y = node->parent;
                while ( node == y->left ) {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }

    }; // __rb_tree_iterator_base

    //RB-tree的正规迭代器
    template < typename Value, typename Ref, typename Ptr >
    struct __rb_tree_iterator : public __rb_tree_iterator_base {
        using value_type = Value;
        using reference = Ref;
        using pointer = Ptr;
        using iterator = __rb_tree_iterator< Value, Value&, Value* >;
        using const_iterator = __rb_tree_iterator< Value, const Value&, const Value* >;
        using self = __rb_tree_iterator< Value, Ref, Ptr >;
        using link_type = __rb_tree_node< Value >*;

        __rb_tree_iterator() {}
        __rb_tree_iterator( link_type x ) {
            node = x;
        }
        __rb_tree_iterator( const iterator& it ) {
            node = it.node;
        }

        reference operator*() const {
            return link_type(node)->value_field;
        }
        pointer operator->() const {
            return &( operator*() );
        }

        self& operator++() {
            increment();
            return *this;
        }
        self operator++( int ) {
            self tmp = *this;
            increment();
            return tmp;
        }

        self& operator--() {
            decrement();
            return *this;
        }
        self operator--( int ) {
            self tmp = *this;
            decrement();
            return tmp;
        } 

        bool operator==( const __rb_tree_iterator& other ) const {
            return node == other.node;
        }
        bool operator!=( const __rb_tree_iterator& other ) const {
            return node != other.node;
        }

    }; //__rb_tree_iterator
    
    //先序遍历
    template < typename T, typename Functor >
    void __DLR_traverse( __rb_tree_node< T >* root_node, Functor do_what ) {
        using node_type = __rb_tree_node< T >;
        if ( root_node == nullptr ) {
            return;
        }
        do_what( root_node );
        __DLR_traverse( (node_type*)root_node->left,do_what );
        __DLR_traverse( (node_type*)root_node->right, do_what );
    }

    template < typename Key, typename Value, typename KeyOfValue, typename Compare, 
    typename Alloc = std::allocator< __rb_tree_node< Value > > >
    class rb_tree {
    public:
        using void_pointer = void*;
        using base_ptr = __rb_tree_node_base*;
        using rb_tree_node = __rb_tree_node< Value >;
        using color_type = __rb_tree_color_type;
        using allocator_type = Alloc;
        allocator_type get_allocator() const {
            return allocator_type();
        }

        using key_type = Key;
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&; 
        using const_reference = const value_type&;
        using link_type = rb_tree_node*;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    protected:
        //分配n个元素的内存
        link_type m_allocate_node( size_type n = 1 ) {
            return  get_allocator().allocate( n );
        }
        //释放内存空间
        void m_deallocate_node( link_type p, size_type n = 1 ) {
            get_allocator().deallocate( p, n );
        }

        //配置节点
        void m_construct_node( link_type p, const value_type& x ) {
            get_allocator().construct( &(p->value_field), x );
        }
        //析构节点
        void m_destroy_node( link_type p ) {
            get_allocator().destroy( &(p->value_field) );
        }
        void m_destory_and_deallocate_node( link_type x ) {
            m_destroy_node( x );
            m_deallocate_node( x );
        }

    protected:
    //mark:rb_tree的数据成员
        size_type node_count; //节点的数量
        link_type header; //哨兵节点
        Compare key_compare; //节点的键值大小比较准则

        link_type& root() const {
            return (link_type&)header->parent;
        }
        link_type& leftmost() const { //最左边的值 最小
            return (link_type&)header->left;
        }
        link_type& rightmost() const { //最右边的值 最大
            return (link_type&)header->right;
        }
        //以下6个函数获取节点x的成员
        static link_type& left( link_type x ) {
            return (link_type&)(x->left);
        }
        static link_type& right( link_type x ) {
            return (link_type&)( x->right );
        }
        static link_type& parent( link_type x ) {
            return (link_type&)( x->parent );
        }
        static reference value( link_type x ) {
            return x->value_field;
        } 
        static const Key key( link_type x ) {
            return KeyOfValue()( value(x) );
        }
        static color_type& color( link_type x ) {
            return (color_type&)( x->color );
        }

        static link_type& left( base_ptr x ) {
            return (link_type&)(x->left);
        }
        static link_type& right( base_ptr x ) {
            return (link_type&)( x->right );
        }
        static link_type& parent( base_ptr x ) {
            return (link_type&)( x->parent );
        }
        static reference value( base_ptr x ) {
            return ((link_type)x)->value_field;
        }
        static const Key key( base_ptr x ) {
            return KeyOfValue()( value(link_type(x)) );
        }
        static color_type& color( base_ptr x ) {
            return (color_type&)(link_type(x)->color );
        }

        static link_type minimum( link_type x ) {
            return (link_type)__rb_tree_node_base::mininum(x);
        }
        static link_type maximum ( link_type x ) {
            return (link_type)__rb_tree_node_base::maxinum(x);
        }
    public:
        using iterator = __rb_tree_iterator< value_type, reference, pointer >;
        using const_iterator =typename  __rb_tree_iterator< value_type, reference, pointer >::const_iterator;

    private:
        iterator __insert( base_ptr x, base_ptr y, const value_type& v );
        link_type __copy( link_type x, link_type y );
        void __erase( link_type x );
        void __double_black( link_type x, link_type x_ori );
        //空红黑树--初始化header节点
        void init() {
            header = m_allocate_node();
            color( header ) = __rb_tree_red;

            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
        }
         //后序遍历：
            void __LRD_traverse( link_type root ) {
            // using node_type = link_type;
            if ( root == nullptr ) {
                return;
            }
            if ( root->left == nullptr && root->right == nullptr ) {
                // std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
                // func( root );
                m_destory_and_deallocate_node( (link_type)root );
                return;
            }

            if ( root->left != nullptr ) {
                __LRD_traverse( (link_type)root->left );
            }
            if ( root->right != nullptr ) {
                __LRD_traverse( (link_type)root->right );
                // func( root );
                m_destory_and_deallocate_node( (link_type)root );
                // std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
            } else {
                // func( root );
                m_destory_and_deallocate_node( (link_type)root );
                // std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
            }
        }

        //先序遍历
        void __DLR_traverse_copy( link_type rt, link_type x ,const rb_tree& other ) {
            if ( x == nullptr ) {
                // m_destroy_node( rt );
                m_deallocate_node( rt );
                return;
            }
            m_construct_node( rt, x->value_field );
            rt->color = x->color;
            if ( x->left != nullptr ) {
                rt->left = m_allocate_node();
                rt->left->parent = rt;
            }
            if ( x->right != nullptr ) {
                rt->right = m_allocate_node();
                rt->right->parent = rt;
            }
            if ( x == other.root() ) {
                rt->parent = header;
                header->parent = rt;
            }
            if ( other.leftmost() == x ) {
                header->left = rt;
            }
            if ( other.rightmost() == x ) {
                header->right = rt;
            }
            __DLR_traverse_copy((link_type)rt->left ,(link_type)x->left, other );
            __DLR_traverse_copy( (link_type)rt->right, (link_type)x->right, other );
        }

    class cnt_key
    {
    public:
        size_t cnt;
        Key key_value;
        cnt_key( size_t c, Key k ) : cnt( c ), key_value( k ) {}
        void  operator()( link_type node ) {
            if ( !Compare()( key_value, key(node) ) && !Compare()( key(node), key_value) ) {
                ++cnt;
            }
        }
    };

    public:
        //mark:rb_tree的构造函数
        rb_tree( const Compare& comp = Compare() ) : node_count( 0 ), key_compare( comp ) {
            init();
        }
        //复制构造函数
        rb_tree( const rb_tree& other ) 
        : node_count( other.node_count ), key_compare( other.key_compare)  {
            init();//节点指针header以完成
            link_type r_node =  m_allocate_node();
            __DLR_traverse_copy( r_node, other.root(), other );
        }
        ~rb_tree() {
            clear();
            m_deallocate_node( header );
        }
        //clear:需要后序遍历
        void clear() {
            __LRD_traverse( root() );
        }
        rb_tree< Key, Value, KeyOfValue, Compare, Alloc >& operator=( const rb_tree< Key, Value, KeyOfValue, Compare, Alloc >& other ) {
            if ( this == &other ) {
                return *this;
            }
            clear();
            link_type r_node = m_allocate_node();
            __DLR_traverse_copy( r_node, other.root(), other );
            node_count = other.node_count;
            key_compare = other.key_compare;
            return *this;
        }

        friend bool operator==( const rb_tree& a, const rb_tree& b ) {
            if ( a.size() != b.size() ) {
                return false;
            }
            if ( &a == &b ) {
                return true;
            }
            auto iter_a = a.begin();
            auto iter_b = b.begin();
            bool rt = true;
            while ( iter_a != a.end() ) {
                if ( *(iter_a++) != *(iter_b++) ) {
                    rt = false;
                    break;
                }
            }
            return rt;
        }

        friend bool operator<( const rb_tree& a, const rb_tree& b ) {
            auto iter_a = a.begin();
            auto iter_b = b.begin();
            // size_t shorter = ( a.size() < b.size() ) ? a.size() : b.size();
            while ( iter_a != a.end() && iter_b != b.end() ) {
                if ( *(iter_a++) != *(iter_b++) ) {
                    break;
                }
            }
            // std::cout << *iter_a << std::endl;
            // std::cout << *iter_b << std::endl;
            // std::cout << *(--iter_a) << std::endl;
            // std::cout << *(--iter_b) << std::endl;
            if ( iter_a == a.end() && iter_b != b.end() ) {
                return true;
            } else {
                if ( *(--iter_a) < *(--iter_b) ) {
                    return true;
                } else {
                    return false;
                }
            }
        }

        Compare key_comp() const {
            return key_compare;
        }
        iterator begin() {
            return leftmost();
        }
        iterator end() {
            return header;
        }
        iterator begin() const {
            return leftmost();
        }
        iterator end() const {
            return header;
        }

        bool empty() const {
            return node_count == 0;
        }
        size_type size() const {
            return node_count;
        }
        size_type max_size() const {
            return size_type(-1);
        }

        void swap( rb_tree& other ) {
            size_type cnt = node_count; node_count = other.node_count; other.node_count = cnt;
            auto ptr_node = header->parent; header->parent = other.header->parent; other.header->parent = ptr_node;
            header->parent->parent = header;
            other.header->parent->parent = other.header;
            ptr_node = header->left; header->left = other.header->left; other.header->left = ptr_node;
            ptr_node = header->right; header->right = other.header->right; other.header->right = ptr_node;
        }
        size_type count( const Key& key_value ) const {
            size_type cnt = 0;
            link_type x = root();
            while ( x != nullptr ) {
                if ( !key_compare( key_value, key(x) ) && !key_compare( key(x), key_value ) ) {
                    cnt_key cout_class( cnt, key_value );
                    auto cout_class_func = std::bind( &cnt_key::operator(), &cout_class, std::placeholders::_1 );
                    std::function< void(link_type) > cnt_functor = cout_class_func;
                    __DLR_traverse( x, cnt_functor );
                    cnt = cout_class.cnt;
                    break;
                } else if ( key_compare( key_value, key(x) ) ) {
                    x = left(x);
                } else if ( key_compare( key(x), key_value ) ) {
                    x = right(x);
                }
            }   
            return cnt;
        }

        //将x插入到rb-tree中，‘保持节点独一无二’
        std::pair< iterator, bool > insert_unique( const value_type& x );
        //将区间[ first, last )插入到红黑树中，保持 ‘独一无二’
        template < typename InputIterator, 
        typename std::enable_if< _is_input_iterator< InputIterator >::value>::type* = nullptr >
        void insert_unique( InputIterator first, InputIterator last ) {
            while ( first != last ) {
                insert_unique( *(first++) );
            }
        }
        //将x插入到rb-tree中， '允许节点重复'
        iterator insert_equal( const value_type& x );

        iterator find( const Key& k ) const;

        //删除函数
        void erase( iterator position ) {
            link_type x = (link_type)position.node;
            if ( x == nullptr ) {
                return;
            }
            if ( left( x ) != nullptr && right(x) != nullptr ) {
                link_type tmp = (link_type)(( ++position).node);//得到中序后继
                x->value_field = tmp->value_field;
                x = tmp; 
            }
            if ( x == leftmost() ) {
                header->left =  (++position).node;
            } else if ( x == rightmost() ) {
                header->right = (--position).node;
            }
            __erase( x );
            --node_count;
        }

        size_type erase( const Key& x ) {
            size_type cnt =  count( x ); 
            size_type tmp = cnt;
            // iterator iter = find( x ) ;
            while( tmp-- ) {
                iterator iter = find( x );
                erase( iter );
            }
            return cnt;
        }
        void erase( iterator first, iterator last ) {
            //fixme:[ first, last ) 迭代器的范围暂时不判断
            while ( first != last ) {
                erase( (first++) );
            }
        }


    }; //rb_tree

    template < typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    void rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::__erase( link_type x ) {
        if ( x == nullptr ) {
            return;
        }
        /**确定删除节点是左孩子还是右孩子亦或是根节点**/
        link_type x_parent = parent( x );
        link_type x_left = left(x);
        link_type x_right = right(x);
        link_type x_sibling = nullptr;
        char l_or_r = 'l';
        if ( parent( x_parent ) == x ) { //删除的节点是根节点
            l_or_r = 'o';
        } else if ( left( x_parent ) == x ) {
            x_sibling = right( x_parent );
            l_or_r = 'l';
        } else if ( right( x_parent ) == x ) {
            x_sibling = left( x_parent );
            l_or_r = 'r';
        }
        /******************************************************************/

        if ( x_left != nullptr && x_right == nullptr ) { //只有左孩子
            parent( x_left ) = x_parent;
            color( x_left ) = __rb_tree_black;
            if ( l_or_r == 'l' ) {
                left( x_parent ) = x_left;
            } else if ( l_or_r == 'r' ){
                right( x_parent ) = x_left;
            } else if ( l_or_r == 'o' ) {
                parent( x_parent ) = x_left;
            }
        } else if ( x_right != nullptr && x_left == nullptr) {//只有右孩子
            parent( x_right ) = x_parent;
            color( x_right ) = __rb_tree_black;
            if ( l_or_r == 'l' ) {
                left( x_parent ) = x_right;
            } else if ( l_or_r == 'r' ){
                right( x_parent ) = x_right;
            } else if ( l_or_r == 'o' ) {
                parent( x_parent ) = x_right;
            }
        } else if ( x_left == nullptr && x_right == nullptr ) { // 没有孩子
            if ( color(x) == __rb_tree_red ) { //删除的节点是红色
                if ( l_or_r == 'l' ) {
                    left( x_parent ) = nullptr;
                } else {
                    right( x_parent ) = nullptr;
                }
            } else {//删除节点是黑色
                __double_black( x, x );
            }
        } 
        m_destory_and_deallocate_node( x );
    }

    //void __double_black( link_type x );
    template < typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    void rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::__double_black( link_type x, link_type x_ori) {
        // link_type y = (link_type)header->parent;
        link_type x_parent = parent( x );
        link_type x_left = left(x);
        link_type x_right = right(x);
        link_type x_sibling = nullptr;
        char l_or_r = 'l';
        if ( parent( x_parent ) == x ) {
            l_or_r = 'o';
        } else if ( left( x_parent ) == x ) {
            x_sibling = right( x_parent );
            l_or_r = 'l';
        } else if ( right( x_parent ) == x ) {
            x_sibling = left( x_parent );
            l_or_r = 'r';
        }
        if ( x_sibling == nullptr ) { //兄弟节点是空
            if ( l_or_r == 'l' ) {
                if ( x == x_ori ) {
                    left( x_parent ) = nullptr;
                }
                if ( color( x_parent ) == __rb_tree_red || x_parent == root() ) {
                    color( x_parent ) = __rb_tree_black;
                } else {
                    __double_black( x_parent, x_ori );
                }
            } else if ( l_or_r == 'r' ) {
                if ( x == x_ori ) {
                    right( x_parent ) = nullptr;
                }
                if ( color( x_parent ) == __rb_tree_red || x_parent == root() ) {
                    color( x_parent ) = __rb_tree_black;
                } else {
                    __double_black( x_parent, x_ori );
                }
            } else {
                if ( x == x_ori ) {
                    parent( x_parent ) = nullptr;
                }
            }
        } else if ( color( x_sibling ) == __rb_tree_black ) { //兄弟节点是黑色
            link_type x_sibling_left = left( x_sibling );
            link_type x_sibling_right = right( x_sibling );
            //兄弟的孩子全是黑色
            if ( (x_sibling_left == nullptr || color( x_sibling_left ) == __rb_tree_black) && 
                 (x_sibling_right == nullptr || color( x_sibling_right) == __rb_tree_black) ) {
                    color( x_sibling ) = __rb_tree_red;
                    if ( l_or_r == 'l' && x == x_ori ) {
                        left( x_parent ) = nullptr;
                    } else if ( l_or_r == 'r' && x == x_ori ){
                        right( x_parent ) = nullptr;
                    }
                    if ( color( x_parent ) == __rb_tree_red || x_parent == root() ) {
                        color( x_parent ) = __rb_tree_black;
                    }  else {
                        __double_black( x_parent, x_ori );
                    }
                } else { //兄弟的孩子至少一个红色
                    if ( l_or_r == 'l' ) {
                        if ( x == x_ori ) {
                            left( x_parent ) = nullptr;
                        }
                        if ( x_sibling_right != nullptr && color( x_sibling_right ) == __rb_tree_red ) { //RR型
                            color( x_sibling_right ) = color( x_sibling );
                            color( x_sibling ) = color( x_parent );
                            color( x_parent ) = __rb_tree_black;
                            __rb_tree_rotate_left( x_parent, header->parent ); 
                        } else { //RL型
                            color( x_sibling_left ) = color( x_parent );
                            color( x_parent ) = __rb_tree_black;
                            __rb_tree_rotate_right( x_sibling, header->parent );
                            __rb_tree_rotate_left( x_parent, header->parent );
                        }
                    } else {
                        if ( x == x_ori ) {
                            right( x_parent ) = nullptr;
                        }
                        if ( x_sibling_left != nullptr && color( x_sibling_left ) == __rb_tree_red ) { //LL
                            color( x_sibling_left ) = color( x_sibling );
                            color( x_sibling ) = color( x_parent );
                            color( x_parent ) = __rb_tree_black;
                            __rb_tree_rotate_right( x_parent, header->parent );
                        } else { //LR
                            color( x_sibling_right ) = color( x_parent );
                            color( x_parent ) = __rb_tree_black;
                            __rb_tree_rotate_left( x_sibling, header->parent );
                            __rb_tree_rotate_right( x_parent, header->parent );
                        }
                    }
                }
        } else { //兄弟节点是红色
            if ( l_or_r == 'l' ) {
                color( x_sibling ) = !color( x_sibling );
                color( x_parent ) = !color( x_parent );
                __rb_tree_rotate_left( x_parent, header->parent );
            } else {
                color( x_sibling ) = !color( x_sibling );
                color( x_parent ) = !color( x_parent );
                __rb_tree_rotate_right( x_parent, header->parent );
            }
            __double_black( x, x_ori );
        }
    }

    template< typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    typename rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::iterator
    rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::find( const Key& k ) const {
        link_type y = header;
        link_type x = root();
        while ( x != nullptr ) {
            if ( !key_compare(key(x), k) ) {
                y = x, x = left(x);
            } else {
                x = right(x);
            }
        }
        iterator j = iterator(y);
        return ( j == end() || key_compare( k, key(j.node) ) ) ? end() : j;
    }

    //插入新节点，键值允许重复
    //返回迭代器，指向新插入的节点
    //函数insert_equal()核心部分是为了确定插入的具体位置；
    //函数__insert()核心部分是对新插入的节点进行旋转，颜色变化等操作，使树仍然符合红黑树
    template < typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    typename rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::iterator 
    rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::insert_equal( const Value& v ) {
        link_type y = header;
        link_type x = root();
        while ( x != nullptr ) {
            y = x;
            x = key_compare( KeyOfValue()(v), key(x) ) ? left(x) : right(x); 
        }
        //x为新插的节点；y为新插入的节点的父节点，v为新插入的值
        return __insert( x, y, v );
    }

    //插入新值：节点键值不允许重复，若重复则插入无效
    //返回值是'pair‘，第一个是迭代器指向新增节点；第二个是表示插入成功与否
    template < typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    std::pair< typename rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::iterator, bool >
    rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::insert_unique( const Value& v ) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while ( x != nullptr ) {
            y = x;
            comp = key_compare( KeyOfValue()(v), key(x) );
            x = comp ? left(x) : right(x);
        }
        iterator j = iterator(y); //迭代器j指向插入新值的父节点
        if ( comp ) {
            if ( j == begin() ) {
                return std::pair< iterator, bool >( __insert( x, y, v ), true ); 
            } else {
                --j;
            }
        }
        if ( key_compare( key(j.node), KeyOfValue()(v) ) ) {
            return std::pair< iterator, bool >( __insert( x, y, v), true );
        }
        return std::pair< iterator, bool >( j, false );
    }
    // template < typename InputIterator >

    /* 
    @param x 旋转点
    */
    inline void __rb_tree_rotate_left( __rb_tree_node_base* x, __rb_tree_node_base*& root ) {
        __rb_tree_node_base* x_parent = x->parent;
        // __rb_tree_node_base* x_left = x->left;
        __rb_tree_node_base* x_right = x->right;

        if ( x_parent->parent == x ) { //x是root
            x_parent->parent = x_right;
            x_right->parent = x_parent;
            x->parent = x_right;
            x->right = x_right->left;
            if ( x_right->left != nullptr ) {

                x_right->left->parent = x;
            }
            x_right->left = x;
        } else if ( x_parent->left == x ) { //x是左孩子
            x_parent->left = x_right;
            x_right->parent = x_parent;
            x->parent = x_right;
            x->right = x_right->left;
            if ( x_right->left != nullptr ) {
                x_right->left->parent = x;
            }
            x_right->left = x;
        } else if ( x_parent->right == x ) { //x是右孩子
            x_parent->right = x_right;
            x_right->parent = x_parent;
            x->parent = x_right;
            x->right = x_right->left;
            if ( x_right->left != nullptr ) {
                x_right->left->parent = x;
            }
            x_right->left = x;
        }
    }
    inline void __rb_tree_rotate_right( __rb_tree_node_base* x, __rb_tree_node_base*& root ) {
        __rb_tree_node_base* x_parent = x->parent;
        __rb_tree_node_base* x_left = x->left;
        // __rb_tree_node_base* x_right = x->right;
        
        if ( x_parent->parent == x ) { //x是root
            x_parent->parent = x_left;
            x_left->parent = x_parent;
            x->parent = x_left;
            __rb_tree_node_base* tmp = x_left->right; //可能为空
            x_left->right = x;
            x->left = tmp;
            if ( tmp ) {
                tmp->parent = x;
            }
        } else if ( x_parent->left == x ) { //x是左孩子
            x_parent->left = x_left;
            x_left->parent = x_parent;
            x->parent = x->left;
            __rb_tree_node_base* tmp = x_left->right; //可能为空
            x_left->right = x;
            x->left = tmp;
            if ( tmp ) {
                tmp->parent = x;
            }
        } else if ( x_parent->right == x ) { //x是右孩子
            x_parent->right = x_left;
            x_left->parent = x_parent;
            x->parent = x_left;
            x->left = x_left->right;
            if ( x_left->right != nullptr ) {
                x_left->right->parent = x;
            }
            x_left->right = x;
        }
    }
    //新插入的节点x的颜色设置，及树的重新平衡问题
    inline void __rb_tree_rebalance( __rb_tree_node_base* x, __rb_tree_node_base*& root ) {
        x->color = __rb_tree_red; //新插入的节点都先设置为红色
        if ( x == root ) {
            x->color = __rb_tree_black;
        } else {
            __rb_tree_node_base* father = x->parent;
            char son_L_or_R = 'L';
            char father_L_or_R = 'L';
            __rb_tree_node_base* grandfather = father->parent;
            __rb_tree_node_base* uncle;
            if ( father == root ) {
            } else {
                if ( father->color == __rb_tree_red ) {
                    if ( father->right == x ) {
                        son_L_or_R = 'R';
                    }
                    if ( grandfather->right == father ) {
                        father_L_or_R = 'R';
                        uncle = grandfather->left;
                    } else {
                        uncle = grandfather->right;
                    }
                    //根据uncle的颜色进行平衡
                    if ( uncle == nullptr ||  uncle->color == __rb_tree_black ) { //叔父节点是黑色或者为nullptr下
                        if ( father_L_or_R == 'R' && son_L_or_R == 'R' ) { //RR型
                            __rb_tree_rotate_left( grandfather, root );
                            grandfather->color = !(grandfather->color);
                            father->color = !(father->color); 
                        } else if ( father_L_or_R == 'R' && son_L_or_R == 'L' ) { //RL型
                            __rb_tree_rotate_right( father, root );//先右旋
                            __rb_tree_rotate_left( grandfather, root );//再左旋
                            //旋转点和旋转中心点 变色
                            grandfather->color = !(grandfather->color);
                            x->color = !(x->color);
                        } else if ( father_L_or_R == 'L' && son_L_or_R == 'R' ) { //LR型
                            __rb_tree_rotate_left( father, root );
                            __rb_tree_rotate_right( grandfather, root );
                            grandfather->color = !(grandfather->color);
                            x->color = !(x->color);
                        } else if ( father_L_or_R == 'L' && son_L_or_R == 'L' ) { //LL型
                            __rb_tree_rotate_right( grandfather, root );
                            grandfather->color = !(grandfather->color);
                            father->color = !(father->color); 
                        }
                    } else { //叔父节点是红色
                        uncle->color = __rb_tree_black;
                        father->color = !(father->color);
                        grandfather->color = !(grandfather);
                        __rb_tree_rebalance( grandfather, root );
                    }
                }
            }
            
        }
    }

    //节点颜色变换和旋转
    /*
    @param _x:新值插入的节点
    @param _y：插入点的父节点
    @param v：新插入的值 
     */
    template < typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
    typename rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::iterator
    rb_tree< Key, Value, KeyOfValue, Compare, Alloc >::__insert( base_ptr _x, base_ptr _y, const Value& v ) {
        link_type x = (link_type)_x;
        link_type y = (link_type)_y;
        link_type z;
        if ( y == header || x != nullptr || key_compare( KeyOfValue()(v), key(y) ) ) {
            //构造了一个新节点
            z = m_allocate_node();
            m_construct_node( z, v );
            left(y) = z;
            if ( y == header ) {
                root() = z;
                rightmost() = z; 
            } else if ( y == leftmost() ) {
                leftmost() = z;
            }
        } else {
            z = m_allocate_node();
            m_construct_node( z, v );
            right(y) = z;
            if ( y == rightmost() ) {
                rightmost() = z;
            }
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __rb_tree_rebalance( z, header->parent ) ;
        ++node_count;
        return iterator(z);
    }

} //thinContainers

#endif // __RB_TREE_H__