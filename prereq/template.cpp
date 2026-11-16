#include<iostream>
using namespace std;

template<typename T>
struct box{
    T value;
    box(T v):value(v){}

    T get() const{
        return value;
    }
};

int main(){
    box<int> a(1);
    box<string>b("qwe");
    cout<<a.get()<<endl;
    cout<<b.get()<<endl;

    return 0;
}