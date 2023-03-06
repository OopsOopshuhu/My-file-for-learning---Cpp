// #include <iostream>
// #include <mutex>
// #include <thread>
// #include <atomic>
// #include <deque>
// #include <condition_variable>

// std::mutex mtx;
// std::deque<int> q;

// std::condition_variable cv;

// //不用多线程，直接一个while循环会大量占用cpu资源
// //使用延时呢，又不能确定延时多久合适。延时过长，消费者不能及时取数据，过短肯呢个饥饿。这时候就需要条件变量解决问题

// //producer
// void task1(){
//     int i = 0;
//     while(true){
//         std::unique_lock<std::mutex> lock(mtx);

//         q.push_back(i);

//         cv.notify_one();//唤醒某一个正在休眠或者等待状态的线程
//         //cv.notify_all();//唤醒所有正在休眠或者等待状态的线程

//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         if (i<999){
//             i++;
//         }
//         else {
//             i = 0;
//         }
//     }
// }
// //consumer1
// void task2(){
//     int data = 0;
//     while (true)
//     {
//         std::unique_lock<std::mutex> lock(mtx);//上锁操作

//         while (q.empty()){
//             cv.wait(lock);//如果为空就让处于休眠状态，这个wait可以帮我们释放掉这个锁
//         }

//         data = q.front();
//         q.pop_front();
//         std::cout << "Get val from que:" << data << std::endl;
//         //std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }

// //consumer2
// void task3(){
//     int data = 0;
//     while (true)
//     {
//         std::unique_lock<std::mutex> lock(mtx);//上锁操作

//         while (q.empty()){
//             cv.wait(lock);//如果为空就让处于休眠状态，这个wait可以帮我们释放掉这个锁
//         }

//         data = q.front();
//         q.pop_front();
//         std::cout << "Get val from que:" << data << std::endl;
//         //std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }

// int main(){
//     std::thread t1(task1);
//     std::thread t2(task2);
//     //两个消费者线程会报错，会出现虚假唤醒
//     //虚假唤醒：当某个条件没有满足，但是线程也被唤醒了,解决方式直接把if改成while
//     std::thread t3(task3);

//     t1.join();
//     t2.join();
//     t3.join();
// }

#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <deque>
#include <condition_variable>

// 生产者消费者模型

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void function_1()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        cond.notify_one(); // 会激活一个等待这个条件的线程
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}
void function_2()
{
    int data = 10;
    while (data != 1)
    {
        std::unique_lock<mutex> locker(mu);
        cond.wait(locker,[](){return !q.empty()});//上句把locker锁住了，这里解锁并休眠，休眠后重新加锁，所以参数有个locker。第二个参数是不希望线程2伪激活
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout
            << "t2 got a val from t1: " << data << std::endl;
        else
        {
            locker.unlock(); // 一直进入这里，持续的循环是非常低效的，怎么高效，唤醒机制
            // std::this_thread::sleep_for(chrono::seconds(10));//很大程度上减少循环的此次数,但时间长短不确定
        }
    }
}

int main()
{
    std::thread t1(function_1);
    std::thread t2(function_2);

    t1.join();
    t2.join();
}