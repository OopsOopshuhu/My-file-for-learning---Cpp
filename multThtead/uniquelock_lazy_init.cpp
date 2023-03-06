#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>

//unique_Lock可以被移动,mutex控制权也被移动了.会消耗更多的计算机性能
//lockguard不可以被移动

class LofFile
{
private:
    std::mutex m_mutex;
    //std::mutex m_mutex_open;
    std::once_flag m_flag;
    std::ofstream f; // 文件流就是输出流，理解成cout，用法同cout

public:
    LofFile(){
        //f.open("log.txt");
    }

    void shared_print(std::string id, int value){
        // {
        //     std::unique_lock<std::mutex> locker(m_mutex_open,std::defer_lock);//写的时候要加锁，谁先拿到锁谁先跑.defer_lock告诉locker，mutex没有被锁
        //     if (!f.is_open()){
        //         f.open("log.txt");
        //     }
        // }
        std::call_once(m_flag, [&]()
                       { f.open("log.txt"); });//这个函数一个线程只会被调用一次
        // std::lock_guard<std::mutex> locker(m_mutex);//写的时候要加锁，谁先拿到锁谁先跑
        std::unique_lock<std::mutex> locker(m_mutex,std::defer_lock);//写的时候要加锁，谁先拿到锁谁先跑.defer_lock告诉locker，mutex没有被锁
        // locker.lock();//为指定的代码加锁
        f << id << value << std::endl;
        // locker.unlock();
        // locker.lock();
        // std::unique_lock<std::mutex> locker2(std::move(locker));
    }
};

void fun1(LofFile& log){
    for (int i = 0; i < 10;i++)
        log.shared_print("From t1:", i);
}

int main(){
    LofFile log;
    std::thread t1(fun1,std::ref(log));
    for (int i = 0; i < 20;i++){
        log.shared_print("From main:", i);
    }

    t1.join();
}