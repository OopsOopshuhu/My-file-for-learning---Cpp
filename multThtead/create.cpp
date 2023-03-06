#include <iostream>
#include <thread>
using namespace std;

void func(int a){
    //cout << "Hello World" << endl;
    //延时的作用，让线程休眠50ms，就是说每隔50ms去打印
    this_thread::sleep_for(chrono::microseconds(50));
}

int main(){//这是一个主线程
    int a = 0;

    // thread 可以理解成class或者create, thread1 是个类，下面这个就是thread1的构造函数，已经完成了线程的创建
    thread thread1(func, a);//不仅创建了子线程，而且开始执行了

    cout << thread1.get_id() << endl;
    cout << thread1.hardware_concurrency() << endl;
    cout << thread1.native_handle() << endl;

    // 如果上面代码下面全空了会报错，因为主线程要一直跑，子线程才会一直跑，主线程下面都没了，直接停了，子线程就会报错了。所以主线程一个true循环就可以保证子线程一直跑
    // 不肯能每次主线程都要写个true循环让子线程活着，所以有了下面代码

    //Solution1
    thread1.join();//表示先跑子线程，子线程执行完毕，才跑下面的主线程的代码，线程会阻塞在这里，使用较多

    //Solution2
    //thread1.detach();//将主子线程完全分隔开，线程不会阻塞在这里。虽然主线程跑完了，子线程还在一直调用函数，随时会关掉。而且如果参数里有指针，就要非常小心
    //所以无法确定子线程的运行情况，使用较少

    cout << "begin to main thread" <<endl;

    // //这个函数是主线程跑的，上面的函数是子线程跑的
    // while (true){
    //     cout << "Cat" << endl;
    //     this_thread::sleep_for(chrono::microseconds(50));
    // }

    // while (true)
    //     ;
}