#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <deque>
#include <condition_variable>

#include <semaphore>

// LeastMaxValue:最小的一个最大值
std::counting_semaphore<3> csem(0);

std::binary_semaphore bsem(0);//内部维护计数器,0是内部计数器的状态

// semaphore release =  condition_variable notify
// semaphore acquire =  condition_variable wait

void task(){
    std::cout << "ready to recv signal: acquire" << std::endl;

    bsem.acquire();//计数器-1,减为ie负的时候会阻塞在这里，直到调用release，计数器增加，继续往下执行

    std::cout << "acquire end" << std::endl;
}
int main(){
    std::thread t(task);

    std::cout << "ready to signal: release" << std::endl;

    bsem.release(2);//计数器增加，增加多少，参数指定，默认1,使用binary_semaphore最好是1 ，因为他是counting_semaphore特化

    std::cout << "signal end" << std::endl;

    t.join();
}