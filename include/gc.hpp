#pragma once

#include<iostream>
#include<vector>
#include<memory>
#include<queue>
#include<utility>
#include<algorithm>

struct GC;

struct Object{
    bool visited=false;
    virtual void trace(GC&) const=0;
    virtual ~Object()=default;
};

struct RootBase{
    virtual Object* get() const=0;
    virtual ~RootBase()=default;
};


struct GC{
    std::queue<Object*>q;
    std::vector<Object*>all;
    std::vector<Object*>survivors;
    size_t minimum_threshold=10000;
    size_t threshold=50000;
    size_t collection_count=0;
    bool stress=false;
    std::vector<RootBase*>handles;

    template<class T, class ... Args>
    T* make(Args&&... args){
        if(stress || all.size()>threshold){
            collect();
        }

        T* obj=new T(std::forward<Args>(args)...);
        track(obj);
        return obj;
    }

    void collect(){
        for(auto& h:handles){
            mark(h->get());
        }
        drain();
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

    void addHandle(RootBase* h){
        handles.push_back(h);
    }

    void removeHandle(RootBase* h){
        auto pos=std::find(handles.begin(),handles.end(),h);
        if(pos!=handles.end()){
            handles.erase(pos);
        }
    }

    ~GC(){
        for(auto& it:all){
            delete it;
        }
    }

};