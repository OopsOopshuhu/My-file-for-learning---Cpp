#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<fstream>
#include<future>
#include<deque>

int factorial(int N){
    int res = 1;
    for (int i = N; i > 1;i--){
        res *= i;
    }
    std::cout << "res is :" << std::endl;
    return res;
}

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

int main(){
    std::thread t1(factorial, 4);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    //下面的代码和上面一样的效果
    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(4);
    std::this_thread::sleep_until(tp);


    //locker也有时间限制的函数
    std::mutex mu;
    std::unique_lock<std::mutex> locker(mu);
    locker.try_lock_for(std::chrono::milliseconds(3));
    locker.try_lock_until(tp);

    //condition_variable依然有时间限制
    std::condition_variable cond;
    cond.wait_for(locker, std::chrono::milliseconds(3));
    cond.wait_until(locker, tp);

    //future也有时间限制
    std::promise<int> p;
    std::future<int> fu = p.get_future();
    fu.wait_for(std::chrono::milliseconds(3));
    fu.wait_until(tp);
}