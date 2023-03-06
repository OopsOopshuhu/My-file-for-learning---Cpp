# 这是一个CMake学习笔记

指令是大小写无关的，参数是有关的
1. set(HELLO hello.cpp)---设置HELLO变量，值为.cpp
2. add_executable(hello main.cpp hello.cpp)
3. add_executable(hello main.cpp ${HELLO})
   
变量使用 ${} 方式取值，但是在IF控制语句中是直接使用变量名

## 重要指令
1. cmake_minimum_required---指定CMake的最小版本要求
   ```cpp
   cmake_minimum_required(VERSION 2.8.3)
   ```
    
2. project---定义工程名称，并可指定工程支持的语言
   ```cpp
   project(HELLOWORLD C++ C Java)
   ```
3. set---显式定义变量
   ```cpp
   set(SRC sayhello.cpp hello.cpp)
   ```
4. include_directories---向工程添加多个特定的头文件搜索路径->向当于指定g++编译器的 -I 参数
   ```cpp
   include_directories(/uer/include/myincludefolder ./include)
   ```
5. link_directories---向工程添加多个特定的库文件搜索路径->相当于g++编译器的 -L 参数‘
   ```cpp
   link_directories(/user/lib/mylibfolder ./lib)
   ```
6. add_library---生成库文件
   ```cpp
   add_library(hello SHARED ${SRC})
   ```
7. add_compile_options---添加编译参数
   ```cpp
   add_compile_options(-Wall -std=c++11 -o2)
   ```
8. add_executable---生成可执行文件
   ```cpp
   add_executable(main main.cpp)
   ```
9. target_link_libraries---为target添加需要的链接的共享库->相当于指定g++编译器 -l 参数
    ```cpp
    target_link_libraries(main hello)
    ```
10. add_subdirectory---向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制存放的位置
    ```cpp
    # 添加src子目录，src中需要有一个CMakeLists.txt
    add_subdirectory(src)
    ```
11. aux__source_directory---发现一个目录下所有的源代码文件并将列表存储在一个变量中，这个指令临时被使用来自动构建源文件列表
    ```cpp
    # 定义SRC变量，其值为当前目录下所有的源代码文件
    aux__source_directory(. SRC)
    # 编译SRC变量所代表的源代码文件，生成main可执行文件
    add_executable(main ${SRC})
    ```
## CMake常用变量
1. CMAKE_CXX_FLAGS---g++编译选项
   ```cpp
   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   ```
2. CMAKE_BUILD_TYPE---编译类型(Debug, Release)
   ```cpp
    # 设定编译类型为debug， 调试时需要选择debug
    set(CMAKE_BUILD_TYPE Debug)
    # 设定编译类型为release， 调试时需要选择release
    set(CMAKE_BUILD_TYPE Release)
   ```
3. CMAKE_BINARY_DIR / PROJECT_BINARY_DIR / <projectname>_BINARY_DIR
   ```cpp
   
   ```
4. CMAKE_CXX_COMPILER---指定C++编译器
5. EXECUTABLE_OUTPUT_PATH---可执行文件输出的存放路径
6. LIBRARY_OUTPUT_PATH---库文件输出的存放路径
   
## CMake编译流程
1. 手动编写CmakeLists.txt
2. 执行命令cmake PATH生成Makefile(PATH 是顶层CMakeLists.txt所在目录)
3. 执行命令make进行编译

### 两种构建方式
1. 内部构建---不推荐
2. 外部构建---将编译输出文件与源文件放到不同目录中
   ```cpp
    # 在当前目录下，创建build文件夹
    mkdir build
    # 进入build文件夹
    cd build 
    # 编译上级目录的CMakeLists.txt， 生成Makefile和其他文件
    cmake ..
    # 执行make命令，生成target
    make
   ```