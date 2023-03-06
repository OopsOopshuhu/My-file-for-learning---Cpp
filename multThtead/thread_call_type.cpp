#include <mutex>
#include <fstream>
#include <future>

class A{
    public:
    void f(int x, char c){

    }
    int operator()(int N){
        return 0;
    }
};

void foo(int x){}

int main(){
    A a;

    //下面两个都是创建子线程，传递一个可调用的对象，一个参数
    std::thread t1(a, 6);//传递a的拷贝给子线程
    std::thread t2(std::ref(a), 6);//传递a的引用给子线程
    std::thread t3(std::move(a), 6);//将a从主移动到子，a在主中已经失效
    std::thread t4(A(), 6);

    std::thread t5(foo, 6);//全局函数构建子线程
    std::thread t6([](int x)
                   { return x * x; },
                   6);//通过lamda函数创建子线程
    std::thread t7(&A::f, a,6, 'c');//传递a的拷贝的成员函数
    std::thread t8(&A::f, &a,6, 'c');//传递a的地址的成员函数

    std::async(std::launch::async, a, 6);//传递a的拷贝给子线程
}