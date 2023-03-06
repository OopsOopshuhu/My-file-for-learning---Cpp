#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

// //目的很简单，但下面的代码太复杂，改造
// // std::mutex mtx;
// // std::condition_variable cv;

// // void task(int a, int b, int& ret){
// //     int ret_a = a * a;
// //     int ret_b = b * 2;
// //     std::unique_lock<std::mutex> lock(mtx);
// //     ret = ret_a + ret_b;
// //     cv.notify_one();//通知主线程我已经算完了，你可以拿到这个结果了
// // }

// // int main(){
// //     //这个为公共资源，因为主线程和子线程都使用到了，所以需要加锁
// //     int ret = 0;

// //     std::thread t(task, 1,2,std::ref(ret));//以引用的方式传入

// //     //do sth else

// //     //get the return cal

// //     std::unique_lock<std::mutex> lock(mtx);

// //     cv.wait(lock);

// //     std::cout << "return val is :" << ret << std::endl;

// //     t.join();
// // }

// //改造完
// void task(int a, std::shared_future<int> b, std::promise<int> & ret){
//     int ret_a = a * a;
//     int ret_b = b.get() * 2;
//     ret.set_value(ret_a + ret_b);

// }

// int main(){
//     //这个为公共资源，因为主线程和子线程都使用到了，所以需要加锁
//     int ret = 0;

//     //主要是想拿到函数的返回值
//     std::promise<int> p_ret;//将他作为参数传递给子线程，然后子线程承诺给一个结果

//     //promise和其他的lock,unique_lock一样都是不可复制的
//     //std::promise<int> p_ret2 = p_ret;//这是错的
//     //std::promise<int> p_ret2 = move(p_ret);//这样是可以的

//     std::future<int> f_ret = p_ret.get_future();//从未来，子线程中拿到结果

//     //promise和future另一个使用方法，当函数参数一个确定了，一个不确定，可以让一个先传进去先算，然后等待，另一个参数确定了，再把另一个参数传进取
//     std::promise<int> p_in;//将他作为参数传递给子线程，然后子线程承诺给一个结果
//     std::future<int> f_in = p_in.get_future();//从未来，子线程中拿到结果

//     std::shared_future<int> s_f = f_in.share();//f_in share过后就可以直接传进去

//     //通过上面两行，就建立好了主子线程连接的通道

//     //因为future.get只能转递一次，所以在多线程的情况下，只能传递不同的future，又因为子线程的多次get操作，所以进程会crush掉
//     //解决方案标准库里的shared_future，值传递，每次都会进行一次复制，所以只会进行一次get操作
//     std::thread t(task, 1,s_f,std::ref(p_ret));//以引用的方式传入
//     std::thread t(task, 1,s_f,std::ref(p_ret));//以引用的方式传入
//     std::thread t(task, 1,s_f,std::ref(p_ret));//以引用的方式传入
//     std::thread t(task, 1,s_f,std::ref(p_ret));//以引用的方式传入

//     //do sth else
//     //...........
//     //
//     p_in.set_value(2);
//     //
//     // get the return cal

//     std::cout << "return val is :" << f_ret.get() << std::endl;
//     //get只能一次，不然会crush

//     t.join();
// }

//future and promise 只能被move

int factorial(std::shared_future<int> f)
{
    int res = 1;
    int N = f.get();//由于没有setval，所以get会抛出异常 std::future_erro::brocken_promise
    for (int i = N; i > 1; i--)
        res *= i;
    std::cout << "result is: " << std::endl;
    return res; // 拿到结果
}
int main()
{
    int x;

    //子线程获取主线程的变量
    std::promise<int> p;
    std::future<int>  f = p.get_future();
    std::shared_future<int> sf = f.share();//share_future可以被拷贝

    // std::thread t1(factorial, 4, std::ref(x));
    std::future<int> fu = std::async(std::launch::async,factorial, sf); // async返回一个future变量
    std::future<int> fu2 = std::async(std::launch::async,factorial, sf); // async返回一个future变量
    std::future<int> fu3 = std::async(std::launch::async,factorial, sf); // async返回一个future变量
    std::future<int> fu4 = std::async(std::launch::async,factorial, sf); // async返回一个future变量

    p.set_value(4);
    // x = fu.get(); // 只能被调用一次
    std::cout << x << std::endl;
    // t1.join();
}