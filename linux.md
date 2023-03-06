# 这是关于linux系统的简单学习总结

## 一. 重要的指令
1. **pwd**  ---打印当前所在目录  
2. **ls**   
   1. ---列出当前目录下的文件/文件夹名称
   2. ---列出指定路径下的...(**ls ./ #表示当前目录**， **ls ../ #表示上一级目录下**， **ls /home #表示绝对路径**)
   3. ---列出文件夹名称，并以指定的格式显示 **ls -lah /home**
      1. -l: 表示list，详细列表展示
      2. -a: 显示所有
      3. -h: 可读性高的形式显示 
   4. ---**ls -lh**查看文件大小
3. **ctrl+l**---清屏
4. **cd**   ---家目录
5. **mkdir**
   1. ---创建目录，在当前文件夹下创建
   2. ---创建多个不存在的目录 **mkdir -p a/b/c** .查看目录结构 **tree a/**
   3. ---一次创建多个文件夹 **mkdir -p a b c**
6. **touch** ---创建新文件
   1. **touch linux.txt**   ---当前目录下创建这个文件
   2. **touch ../linux**    ---在上级目录创建linux文件
   3. **touch file file.txt-**--当前目录创建两个文件
7. **rm**---删除文件/目录
   1. **rm myfile-**--删除当前目录下myfile文件
   2. **rm /user/myfile**---删除user路径下的文件
   3. **rm -rf myfolder**---删除当前路径下目录
   4. **rm *,out**---把.out结尾的文件全部删掉
8. **cp**---复制文件夹/文件到指定位置
   1. **cp /home/bing/myfile ./** ---复制文件
   2. **cp -r /home/bing/myfolder /**---复制文件夹
9. **mv**---移动文件或者重命名
   1.  **mv myfile /myfile**---当前目录下文件移动到根目录下
   2.  **mv myfolder /myfolder**---移动文件夹到根目录下
   3.  **mv myfile myfile007**---原地移动并重命名
10. **man**---查看命令指令手册，**按q**退出
11. **reboot**---重启linux系统
12. **shutdown -h now**---立即关机
13. **shift+g**---直接到最后一行

### g++重要编译参数
1. **-g** ---编译带调试信息的可执行文件
```cpp
g++ -g test.cpp
```
2. **-O[n]**---优化源代码
   1. **-O**---做基本优化
   2. **-O1**---缩小代码长度和执行时间
   3. **-O2**---除O1外再做一些指令调整
   4. **-O3**---包括循环展开等其他相关的优化工作
```cpp
g++ -O2 test.cpp
```
3. **-l 和 -L**---指定库文件 | 指定库文件路径
```cpp
# -l 后面紧跟库名，如 /lib /usr /lib
# 链接 glog 库

g++ -lglog test.cpp

# 如果库文件不在上面三个目录里，需要 -L 指定库文件所在目录
# 链接 mytest 库， libmytest.so 在 /home/bing/mytestlibfolder 目录下

g++ -L/home/bing/mytestlibfolder -lmytest test.cpp
```
4. **-I**---指定头文件搜索目录
```cpp
g++ -I/myinclude test.cpp
```
5. **-Wall**---打印警告信息
```cpp
g++ -Wall test.cpp
```
6. **-w**---关闭警告信息
7. **-std=c++11**---设置编译标准
```cpp
g++ -std=c++11 test.cpp
```
8. **-o**---指定输出文件名
```cpp
g++ test.cpp -o test
```
9. **-D**---定义DEBUG宏

## 二. 文件编辑
### Vim[file]
**vim linux007**---进入这个文件，**按i**进入可以编辑模式，结束之后**esc,shift+:+w**保存，**:q**
```cpp
# 预处理+编译+汇编+链接，生成可执行文件，之后可以删掉
g++ test.cpp -o test

# 生成优化的可执行文件
g++ test.cpp -O2 -o test

# 执行语句，加个time可以看到执行时间
time ./test
```
**set ts=4**---更改tab缩进为4
**set nu**---调出行号，方便调试

### gedit[file]
**gedit linux007**---和记事本一样
### nano[file]

## 三. 实操
文件/文件夹都建好了之后，用tree看一眼当前结构
![](2023-02-14-12-13-17.png)
```cpp
# 生成可执行文件
g++ main.cpp src/swap.cpp -Iinclude
g++ main.cpp src/swap.cpp -Iinclude -Wall -std=c++11 -o res
```
### 生成库文件并编译
1. 链接静态库
   1. 封装好一个函数，做成库文件，g++在编译运行的时候就不用把这个函数再编译了，以后使用这个函数就和printf一样方便
   2. 编译的时候只需要链接这个库就可以
   3. 静态库生成的可执行文件包含了静态库，在运行的目标机上不需要再存储这个静态库
   ```cpp
   # src目录
   # 汇编---生成swap.o文件。 **-c**生成一个.o的二进制文件
   g++ swap.cpp -c -I../include

   # 生成静态库 libswap.a，把.o文件归档.a的静态库文件
   ar rs libswap.a swap.o

   # 返回上级目录
   # 链接---生成可执行文件：staticmain
   g++ main.cpp -Iinclude -lswap -Lsrc -o staticmain
   -lswap自动会链接libswap.a静态库，因为这个swap函数已经被我们封装好并且归档在了库里。又因为不在当前目录下，所以要-Lsrc
   ```
2. 链接动态库
   1. 生成的可执行文件不包含库文件，只是留好了记号，运行时去寻找什么库文件，要求目标机中有此动态库文件
   ```cpp
   # src目录
   # 生成动态库 libswap.so
   g++ swap.cpp -I../include -fPIC -shared -o libswap.so
   # 上面的指令等价于下面两条指令
   g++ swap.cpp -I../include -c -dPIC
   g++ -shared -o libswap.so swap.o

   # 返回上级目录
   # 链接---生成可执行文件：sharemain
   g++ main.cpp -Iinclude -lswap -Lsrc -o sharemain
   # 动态库生成的可执行文件不能直接运行只是在运行的时候去加载，因为动态库路径是在src
   # 运行可执行文件
   LD_LIBRARY_PATH=src ./sharemain
   ```
    ![](2023-02-14-13-38-32.png)

## 四. GDB调试器
### GDB常用调试命令
调试开始：执行 gdb exefilename
1. gdb help(h)---查看命令帮助
2. gdb **run(r)**---重新开始运行文件
3. gdb **start**---单步执行，运行程序，停在第一行语句
4. gdb **list(l)**---查看源代码(list-n，从第n行开始查看代码，list+func：查看具体函数名)
5. gdb **set**---设置变量的值
6. gdb **next(n)**---单步调试(逐过程，函数直接执行)
7. gdb **step(s)**---单步调试(逐语句，跳入自定义函数内部执行)
8. gdb backtrace(bt)---查看函数调用的栈帧和层级关系
9. gdb frame(f)---切换函数的栈帧
10. gdb **info(i)**---查看函数内部局部变量的数值
11. gdb **finish**---结束当前函数，返回函数调用点
12. gdb **continue(c)**---继续运行
13. gdb **print(p)**---打印值及地址
14. gdb **quit(q)**---退出gdb
15. gdb **break+num(b)**---在第num行设置断点
16. gdb **info breakpoints(i b)**---查看当前设置的所有断点
17. gdb **delete breakpoints num(d)**---删除第num个断点
18. gdb **display**---追踪具体变量值
19. gdb **undisplay**---取消追踪变量值
20. gdb **watch**---被设置观察点的变量发生修改时，打印显示
21. gdb **i watch**---显示观察点
22. gdb **enable breakpoints**---启用断点
23. gdb **disable breakpoints**---禁用断点
24. gdb x---查看内存x/20xw 显示20个单元，16进制，4字节每单元
25. gdb run argv[1] argv[2]---调试时命令行传参
26. gdb set follow-fork-mode child#Makefile---项目管理：选择跟踪父子进程(fork())
Tips:编译程序时需要加上-g，之后才能用gdb调试：g++ -g main.cpp -o main。 回车键重复上一命令

