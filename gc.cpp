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
    std::vector<std::function<Object*()>>roots;

    void addRoot(std::function<Object*()> f){
        roots.push_back(f);
    }

    void collect(){
        for(auto& f:roots){
            mark(f());
            drain();
        }
        sweep();
    }

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

    ~GC(){
        for(auto& it:all){
            delete it;
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
    ~TreeNode(){
        std::cout<<"freeing: "<<val<<std::endl;
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

    TreeNode* other=new TreeNode(10);
    gc.track(other);
    other->left=new TreeNode(11);
    gc.track(other->left);


    gc.addRoot([&] {
        return root;
    });
    gc.addRoot([&]{
        return other;
    });
    int i=0;

    std::cout<<"run: "<<++i<<std::endl;
    gc.collect();
    std::cout<<"all: "<<gc.all.size()<<std::endl<<std::endl;

    other=nullptr;

    std::cout<<"run: "<<++i<<std::endl;
    gc.collect();
    std::cout<<"all: "<<gc.all.size()<<std::endl<<std::endl;

    return 0;
}