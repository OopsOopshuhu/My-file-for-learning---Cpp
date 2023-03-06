#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>

int factorial(int N)
{
    int res = 1;
    for (int i = N; i > 1; i--)
    {
        res *= i;
    }
    return res;
}

// 这里就建立了一个函数队列
std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

// 实现异步调用
void thread_1()
{
    std::packaged_task<int()> t;
    {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, []()
                  { return !task_q.empty(); });
        t = std::move(task_q.front());
    }
    t();
}

int main()
{
    // packaged_task可以关联一个对象连接一个future，通过future来异步获取可调用对象执行的结果

    std::thread t1(thread_1);

    std::packaged_task<int()> t(std::bind(factorial, 6)); // bind创建一个函数对象

    std::future<int> ret = t.get_future();

    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(t));
        cond.notify_one();
    }

    int val = ret.get();
    std::cout << "main thread val:" << val << std::endl;
    t1.join();
}