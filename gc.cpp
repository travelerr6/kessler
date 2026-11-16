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
    size_t minimum_threshold=10000;
    size_t threshold=50000;
    size_t collection_count=0;

    template<class T, class ... Args>
    T* make(Args&&... args){
        if(all.size()>threshold){
            collect();
        }

        T* obj=new T(std::forward<Args>(args)...);
        track(obj);
        return obj;
    }

    void addRoot(std::function<Object*()> f){
        roots.push_back(f);
    }

    void collect(){
        for(auto& f:roots){
            mark(f());
            drain();
        }
        sweep();
        threshold=std::max(minimum_threshold,2*all.size());
        collection_count++;
    }

    void collections(){
        std::cout<<"collection count = "<<collection_count<<std::endl;
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
        // std::cout<<"freeing: "<<val<<std::endl;
    }
};

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