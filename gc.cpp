#include "include/pch.h"

struct GC;

struct Object{
    bool visited=false;
    virtual void trace(GC&) const=0;
    virtual ~Object()=default;
};

struct GC{
    std::queue<Object*>q;
    std::vector<Object*>all;
    std::vector<Object*>survivors;

    void track(Object* o){
        all.push_back(o);
    }

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
    void sweep(){
        for(auto& it:all){
            if(!it->visited){
                delete it;
            }
            else{
                it->visited=false;
                survivors.push_back(it);
            }
        }
        all=survivors;
        survivors.clear();
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
    ~TreeNode(){
        std::cout<<"freezing: "<<val<<std::endl;
    }
};

int main(){
    GC gc;
    TreeNode* root=new TreeNode(1);
    gc.track(root);
    root->left=new TreeNode(2);
    gc.track(root->left);
    root->right=new TreeNode(3);
    gc.track(root->right);

    TreeNode* ran=new TreeNode(99);
    gc.track(ran);

    std::cout<<"all: "<<gc.all.size()<<std::endl;
    
    gc.mark(root);
    gc.drain();

    std::cout<<"root: "<<root->visited<<std::endl;
    std::cout<<"root left: "<<root->left->visited<<std::endl;
    std::cout<<"root right : "<<root->right->visited<<std::endl;

    std::cout<<"ran: "<<ran->visited<<std::endl;

    gc.sweep();

    return 0;
}