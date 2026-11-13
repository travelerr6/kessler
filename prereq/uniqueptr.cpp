#include<iostream>
#include<vector>
#include<memory>
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
struct square:shape{
    double s;
    square(double s):s(s){}
    double area() const override{
        return s*s;
    }
    ~square() override {
        cout<<"square class destroyed side = "<<s<<endl;
    }
};

int main(){
    vector<unique_ptr<shape>>shapes;
    shapes.push_back(make_unique<circle>(3));
    shapes.push_back(make_unique<square>(4));
    shapes.push_back(make_unique<circle>(1));

    for(auto& s:shapes){
        cout<<s->area()<<endl;
    }

    return 0;
}