#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

//mutex是一个线程同步原语
mutex mtx1;//创建一个全局互斥量

//如果有多个线程同时访问一个公共资源，就需要对这个资源进行一下保护
//如果不保护，就不是一个线程安全的操作，所以要对不安全的代码段进行保护，引出mutex概念

//有两种死锁的情况
/*
1 未解锁，return出来了
lock_guard是一个模板类，实例化的是mutex类，构造函数里传入互斥量
处理：lock_guard<mutex> lock(mtx1);在它的构造函数进行上锁，析构解锁
     lock_guard<mutex> lock(mtx2);
     然后也不用再加锁解锁，都由lock_guard完成
*/

unique_lock<mutex> lock3(mtx1);
//lock_guard只有超出作用于才会析构
//unique可以提前释放


/*
2 都要对方的锁
处理：1、同时对一个资源上锁
    2、lock(mtx1,mtx2)
*/


//另一个解决多线程线程不安全的方法
//原子变量automic
//不用使用mutex或者锁来实现对资源的原子操作
atomic<int> globalVariable = 0;

int main(){

}