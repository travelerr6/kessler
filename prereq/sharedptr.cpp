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

int main(){\
    shared_ptr<shape>p1=make_shared<circle>(1);
    cout<<p1.use_count()<<endl;
    {
        vector<shared_ptr<shape>>p2;
        p2.push_back(p1);
        
        {
            shared_ptr<shape>p3=p1;
            cout<<p1.use_count()<<endl;
        }
        cout<<p1.use_count()<<endl;
    }
    cout<<p1.use_count()<<endl;

    return 0;
}