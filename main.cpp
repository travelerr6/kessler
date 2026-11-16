#include<iostream>
#include "include/gc.hpp"
#include "include/tree_node.hpp"

int main(){
    GC gc;

    TreeNode* root=nullptr;
    TreeNode* other=nullptr;
    gc.addRoot([&] {
        return root;
    });
    gc.addRoot([&]{
        return other;
    });

    root=gc.make<TreeNode>(1);
    root->left=gc.make<TreeNode>(2);
    root->right=gc.make<TreeNode>(3);

    other=gc.make<TreeNode>(10);
    other->left=gc.make<TreeNode>(11);


    for(int i=0;i<1000000;i++){
        gc.make<TreeNode>(i);
    }

    gc.collect();
    gc.collections();
    std::cout<<"all: "<<gc.all.size()<<std::endl<<std::endl;

    other=nullptr;

    gc.collect();
    gc.collections();
    std::cout<<"all: "<<gc.all.size()<<std::endl<<std::endl;

    return 0;
}