#pragma once
#include "gc.hpp"

struct ListNode:Object{
    std::vector<Object*>items;
    ListNode(){} 
    //ListNode(Object* o) shouldnt be written beacuse it cause a major loophole int the code, lets say we call gc.make<ListNode>(gc.make<TreeNode>(1)), the compiler will process the creation of the TreeNode with the value 1, and it stores the returned pointer in the all vector via track() function, now the creation of the ListNode come into the picture, now imagine the threshold gets trigerred without creation of the ListNode so it will free up the TreeNode from the memory, and the ListNode will then be created with items having pointer pointing to nothing, called dangling pointer
    void trace(GC& gc) const override{
        for(auto& it:items){
            gc.mark(it);
        }
    }
    ~ListNode(){}
};