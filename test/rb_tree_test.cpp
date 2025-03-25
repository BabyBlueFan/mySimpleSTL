#include "../header/rb_tree.h"

using namespace thinContainers;

struct value_key {
    int operator()( int value ) {
        return value;
    }
};
template < typename T >
void LDR_print( __rb_tree_node< T >* root ) {
    using node_type = __rb_tree_node< T >;
    if ( root == nullptr ) {
        return;
    }
    LDR_print( (node_type*)root->left  );
    std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
    LDR_print( (node_type*)root->right );
}

//后序遍历：
template < typename T >
void LRD_print( __rb_tree_node< T >* root ) {
    using node_type = __rb_tree_node< T >;
    if ( root == nullptr ) {
        return;
    }
    if ( root->left == nullptr && root->right == nullptr ) {
        std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
        return;
    }

    if ( root->left != nullptr ) {
        LRD_print( (node_type*)root->left );
    }
    if ( root->right != nullptr ) {
        LRD_print( (node_type*)root->right );
        std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
    } else {
        std::cout << "value: " << root->value_field << " color: " << root->color << std::endl;
    }
}

//前序遍历
template < typename T >
    void DLR_print( __rb_tree_node< T >* x ) {
        using node_type = __rb_tree_node< T >;
        if ( x == nullptr ) {
            return;
        }
        std::cout << "value: " << x->value_field << " color: " << x->color << std::endl;
        DLR_print( (node_type*)x->left );
        DLR_print( (node_type*)x->right );
    }

int main( void ) {

    

    rb_tree< int, int, value_key, std::less< int > > rbTree;
    //构建红黑树：17 18 23 34 27 15 9 6 8 5 25 
    rbTree.insert_unique( 17 );
    rbTree.insert_unique( 18 );
    rbTree.insert_unique( 23 );
    // rbTree.insert_equal( 23 );
    rbTree.insert_unique( 34 );
    rbTree.insert_unique( 27 );
    rbTree.insert_unique( 15 );
    // rbTree.insert_equal( 23 );
    // rbTree.insert_equal( 23 );
    // rbTree.insert_equal( 23 );
    rbTree.insert_unique( 9 );
    rbTree.insert_unique( 6 );
    rbTree.insert_unique( 8 );
    rbTree.insert_unique( 5 );
    rbTree.insert_unique( 25 );

    std::cout << rbTree.count( 23 ) << std::endl;
    __rb_tree_iterator iter = rbTree.end();
    __rb_tree_node< int >* ptr = (__rb_tree_node< int >*)iter.node->parent;
    LDR_print( ptr );
    __rb_tree_iterator tmpIter = rbTree.begin();
    std::cout << *tmpIter << std::endl;
    /* ++tmpIter;
    std::cout << *tmpIter << std::endl;
    ++tmpIter;
    std::cout << *tmpIter << std::endl;
    ++tmpIter;
    std::cout << *tmpIter << std::endl; */
    /* ++tmpIter;
    std::cout << *tmpIter << std::endl; */
    // ++tmpIter;
    std::cout << std::endl;
    rbTree.erase( tmpIter );
    LDR_print( ptr );
   /*  rb_tree< int, int, value_key, std::less< int > > rbTree_copy( rbTree ); //复制构造函数

    std::cout << *rbTree.begin() << "----" << *rbTree_copy.begin() << std::endl;
    std::cout << *(--rbTree.end()) << "----" << *(--rbTree_copy.end()) << std::endl;
    std::cout << rbTree.size() << "----" << rbTree_copy.size() << std::endl;
 */

    // __rb_tree_iterator iter0= rbTree_copy.end();
    // __rb_tree_node< int >* ptr0= (__rb_tree_node< int >*)iter0.node->parent;
    /* LDR_print( ptr );
    std::cout << std::endl;
    // LRD_print( ptr );
    DLR_print( ptr );

    LDR_print( ptr0 );
    DLR_print( ptr0 );
 */
    return 0;
}