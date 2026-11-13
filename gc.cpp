#include "include/pch.h"

struct GC;

struct Object{
    bool visited=false;
    virtual void trace(GC&) const=0;
    virtual ~Object()=default;
};

struct GC{
    std::queue<Object*>q;

    void mark(Object* o){
        if(o){
            if(!o->visited){
                o->visited=true;
                q.push(o);
            }    
        }
    }
    void drain(){
        while(!q.empty()){
            Object* o=q.front();
            q.pop();
            o->trace(*this);
        }
    }
};

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
};

int main(){
    TreeNode* root=new TreeNode(1);
    root->left=new TreeNode(2);
    root->right=new TreeNode(3);
    GC gc;
    gc.mark(root);
    gc.drain();

    std::cout<<"root: "<<root->visited<<std::endl;
    std::cout<<"root left: "<<root->left->visited<<std::endl;
    std::cout<<"root right : "<<root->right->visited<<std::endl;

    TreeNode* ran=new TreeNode(99);
    std::cout<<"ran: "<<ran->visited<<std::endl;

    return 0;
}