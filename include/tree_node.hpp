#pragma once
#include "gc.hpp"

struct TreeNode:Object{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v){
        val=v;
        left=nullptr;
        right=nullptr;
    }
    void trace(GC& gc) const override{
        gc.mark(left);
        gc.mark(right);
    }
    ~TreeNode(){
        std::cout<<"freeing: "<<val<<std::endl;
    }
};