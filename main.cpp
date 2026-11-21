#include <iostream>
#include "include/handle.hpp"
#include "include/tree_node.hpp"

int main(){
    GC gc;
    Handle<TreeNode> root(gc);
    Handle<TreeNode> other(gc);
    root = gc.make<TreeNode>(1);
    root->left  = gc.make<TreeNode>(2);
    root->right = gc.make<TreeNode>(3);
    other = gc.make<TreeNode>(10);
    other->left = gc.make<TreeNode>(11);

    gc.collect();
    std::cout << "after 1st: " << gc.all.size() << "\n";
    other = nullptr;
    gc.collect();
    std::cout << "after 2nd: " << gc.all.size() << "\n";

    return 0;
}