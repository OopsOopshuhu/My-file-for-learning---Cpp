## 项目笔记
#### 1. 使用的是有栈还是无栈协程？
  无栈协程，new出来的空间
#### 2. ucontext怎么实现上下文切换
  * Xfiber create a fiber
  * fiber register Xfiber and push into ready_list(相当于就绪队列， running_list相当于执行队列)，等待队列是用map做的，map就是waiting_list
  * init fiber_ctx depend on sched_ctx
  * makecontext update ctx and bind start func entry 
  * **swapcontext** save sched_ctx and active current coroutine_ctx
  * if yield , save coroutine_ctx and active sched_ctx
#### 3. 怎么把协程用在网络框架---编码方式是同步的，但是执行方式是异步的
  > 同步编码： listen->accept->read->write
  * create coroutine for serving
  * listen local port
  * loop true
  * accept client connect in loop
  * if connect success , create another coroutine for processing this request
    > 来一个连接就处理，来一个就处理，每一个连接器都是一个协程，不会阻塞主流程
#### 4. listen
  * create a fd
  * set fd on non_block model
  * bind local port
  * listen
#### 5. 怎么解决网络IO问题，主动让出CPU(没有连接的时候不要一直accept)
  * if successful connected , schedular create fiber
  * accept a client_fd and return a new fd --- epoll_createl(0);
  * **register** fiber and fd as a bag to (**map<fd, waitingfibers> io_waiting_fiber**) int schedular's(epoll) --- epoll_ctl(); fd->rbt, map->waiting_list
  * check ready_list --- epoll_wait()(keep running if it has tasks until finished)
  * check epoll_fd(map) whether a fd triggered --- epoll_wait()
  * if no fd triggered , **switch** to schedular --- swapcontext 
  * // 下面是map等待队列的作用
  * if a fd triggered , according event_fd to check which coroutine belongs to this fd in map
  * if event_fd is not in map , continue --- 不知道从哪来的事件或者意外触发
  * if find , judge read or write ascertain which fiber
  * push this fiber to read_list (awake)
  * **switch** to process_fiber --- swapcontext , save schedular_ctx and switch to process_fiber
  > 没有事件来的标志是 errno = eagain ， 一个register 一个switch
#### 6. 怎么实现fd注册调度器---map存储(等待队列)
  * map<fd, waitingfibers> io_waiting_fiber；
    > 这个 waitingfibers 是个结构体(read_fiber, write_fiber)，因为fd有读有写，cur_fiber可以根据到来的fd来决定cur_fiber赋给读协程还是写协程(拷贝构造)
  ```cpp
    register_func(int fd, bool is_write){
        // 1. check whether this fiber is registered
        // 2. if not , judge whether it is write or read and copy construct cur_fiber
        // 3. insert
        // 4. fd register tp epoll --- epoll_ctl (edge trigger)
        // if it has already ,  update fiber
        // 一共做两件事， 1 bind加到map， 2 fd to rbt
    }
  ```
#### 9. 实现协程代理
* 添加一个connection类
* 两个连接进来
* 创建一个协程去处理两个任务
#### 10. 实现超时机制
超时时间转化为过期时间---time_now+timeout(相当于你几点死，而不是还能活多长时间)
* 在设置事件的时候多加一个过期时间参数
* 怎么让调度器去管理这些任务？
  * 1. map< int64_t, set< Fiber *>> expired_events --- key: 过期时间， value: 这个时间点过期的fiber集合
  * 2. 过期时间要和fd绑定
  > expired_events 相当于调度器的超时队列
  * 在注册的时候判断过期时间>0, 再把当前协程加到过期事件(map)里
  * 在dispatch里进行调度，当有事件触发，如果超时队列的过期时间大于当前时间，激活协程。然后从队列删掉，同时也要把等待队列里的相应协程删掉
  > 之前是事件触发直接丢就绪，然后删等待，现在是丢到超时队列，没超时的丢就绪，然后删等待
  * 在read和write里面进行判断，如果当前时间大于过期时间，就直接返回-1,表示超时。同时把超时队列和等待队列的协程删掉，其实就是断了连接
> 超时定时器只和等待队列相关，连接到来，就丢到超时队列，有事件触发，激活协程，删除等待队列，删除超时队列。没有事件触发，读写超时，直接断开连接
#### 11. 信号量机制
设置等待队列，拿锁的申请到协程，没拿到的等待，协程直接串行化预防死锁
#### 8. 一些小技巧
##### 1. XFiber 是线程级别的，是广义的线程，这里可以设置一个静态接口直接拿到这个实例
```cpp
static XFiber *xfiber(){
    static thread_local XFiber xf;
    return &xf;
}
// 通过指针函数拿到实例，这样做的好处是当有多个实例的时候方便通过这个统一的接口存取
XFiber *xfiber = XFiber::xfiber();
```
##### 2. 出现问题，每次唤醒协程唤醒了两次
找到原因：每次把唤醒协程加到就绪队列，并没有把等待队列的协程删掉
解决方式：封装唤醒方法
1. 唤醒协程加到就绪队列
2. 等待队列踢掉
   * 方式一： for循环map里一个个找，找到同一个唤醒协程，删掉对应fd --- 效率很低
   * 方式二： 定义一个fd(注册的时候初始化)， 标记唤醒协程fd(但是fd的读写没办法区分) 所以 ！仍然是定义一个结构体(属性设为两个数组，分别代表读写，有多少fd放多少fd)，这样就可以监听多个fd(当协程有代理功能，需要监听两个fd)，
3. epoll里面踢掉
   
> 后面可以做不用剔除的优化
#### 3. 
