#pragma once
#include "gc.hpp"

template<class T>
struct Handle:RootBase{
    GC& gc;
    T* ptr=nullptr;

    Handle(GC& g):gc(g){
        gc.addHandle(this);
    }

    ~Handle(){
        gc.removeHandle(this);
    }

    Handle(const Handle&)=delete;

    Handle& operator=(const Handle&)=delete;

    Handle& operator=(T* p){
        ptr=p;
        return *this;
    }

    T* operator->() const {
        return ptr;
    }

    Object* get() const override{
        return ptr;
    }

};