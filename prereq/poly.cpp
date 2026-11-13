#include<iostream>
using namespace std;

struct shape{
    virtual double area()const=0;
    virtual ~shape(){
        cout<<"shape class destroyed"<<endl;
    };
};

struct circle:shape{
    double r;
    circle(double r):r(r){}
    double area() const override{
        return 3.14*r*r;
    }
    ~circle() override {
        cout<<"circle class destroyed radius = "<<r<<endl;
    }
};

int main(){
    circle c(1);
    circle c1(2);
    cout<<c.area()<<endl;

    shape* s=new circle(3);
    cout<<s->area()<<endl;
    delete s;

    return 0;
}