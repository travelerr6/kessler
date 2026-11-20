#include <iostream>
#include "include/handle.hpp"
#include "include/tree_node.hpp"

int main(){
    GC gc;
    {
    Handle<TreeNode> t(gc);
    t = gc.make<TreeNode>(42);
    }
    std::cout << "before: " << gc.all.size() << "\n";
    gc.collect();
    std::cout << "after: " << gc.all.size() << "\n";
    //std::cout << t->val << "\n";      // expect 42
    return 0;
}