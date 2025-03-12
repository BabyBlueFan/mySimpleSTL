#ifndef __RB_TREE_H__
#define  __RB_TREE_H__

#include <iostream>
#include <memory>
#include <new>
#include <type_traits>

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
                while ( y->right == nullptr ) {
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

    protected:
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

    private:
        iterator __insert( base_ptr x, base_ptr y, const value_type& v );
        link_type __copy( link_type x, link_type y );
        void __erase( link_type x );
        
        //空红黑树
        void init() {
            header = m_allocate_node();
            color( header ) = __rb_tree_red;

            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
        }

    public:
        rb_tree( const Compare& comp = Compare() ) : node_count( 0 ), key_compare( comp ) {
            init();
        }
        ~rb_tree() {
            clear( (link_type)header->parent );
            m_deallocate_node( header );
        }

        void clear( link_type x ) {
            if ( x == nullptr ) {
                return;
            }
            clear( left(x) );
            m_destroy_node(x);
            m_deallocate_node(x);
            clear( right(x) );
        }

        rb_tree< Key, Value, KeyOfValue, Compare, Alloc >& operator=( const rb_tree< Key, Value, KeyOfValue, Compare, Alloc >& other );

        Compare key_comp() const {
            return key_compare;
        }
        iterator begin() {
            return leftmost();
        }
        iterator end() {
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

        //将x插入到rb-tree中，‘保持节点独一无二’
        std::pair< iterator, bool > insert_unique( const value_type& x );
        //将x插入到rb-tree中， '允许节点重复'
        iterator insert_equal( const value_type& x );

        iterator find( const Key& k ) const;
    }; //rb_tree

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