#include<iostream>
#include "include/gc.hpp"
#include "include/tree_node.hpp"
#include "include/list_node.hpp"

int main(){
    GC gc;
    TreeNode* t=gc.make<TreeNode>(42);   // an unregistered local
    gc.collect();                        // frees node 42 (no roots), and nothing reuses its memory
    std::cout<<t->val<<"\n";             // read the freed node
    
    return 0;
}