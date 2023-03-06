#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>

class LofFile
{
private:
    std::mutex m_mutex;
    std::ofstream f;//文件流就是输出流，理解成cout，用法同cout

public:
    LofFile(){
        f.open("log.txt");
    }

    void shared_print(std::string id, int value){
        std::lock_guard<std::mutex> locker(m_mutex);//写的时候要加锁，谁先拿到锁谁先跑
        f << id << value << std::endl;
    }

    //f在mutex的保护下，不能添加一个函数来返回一个f，破坏了保护等级
    //std::ofstream &Getstream() { return f; } //这样就在互斥对象的保护外使用f了。一旦调用这个函数，f就拿到了，就可以操作了，所以不能这样操作
    /*void precess(void fun(std::ofstream&)){//不能将f作为函参传递出去
        fun(f);//f依然暴露在类外
    }*/
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