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

int main( void ) {

    

    rb_tree< int, int, value_key, std::less< int > > rbTree;
    //构建红黑树：17 18 23 34 27 15 9 6 8 5 25 
    rbTree.insert_equal( 17 );
    rbTree.insert_equal( 18 );
    rbTree.insert_equal( 23 );
    // rbTree.insert_equal( 23 );
    rbTree.insert_equal( 34 );
    rbTree.insert_equal( 27 );
    rbTree.insert_equal( 15 );
    rbTree.insert_equal( 9 );
    rbTree.insert_equal( 6 );
    rbTree.insert_equal( 8 );
    rbTree.insert_equal( 5 );
    rbTree.insert_equal( 25 );
    __rb_tree_iterator iter = rbTree.end();
    __rb_tree_node< int >* ptr = (__rb_tree_node< int >*)iter.node->parent;
    LDR_print( ptr );
    return 0;
}