### 静态库 动态库
g++ *.cpp -o * 默认使用动态链接

g++ -static *.cpp -o * 使用静态链接

//静态链接在编译阶段就把需要的库函数加入到编译中形成可执行文件；动态链接在使用某些功能的时候再加入
### 静态库和动态库制作
``` 
1.静态库： 先把所有的.cpp 文件执行 g++ -c *.cpp -o *.o
          在对所有的.o文件执行 ar -rcs libxxx.a *.o 生成libxxx.o的静态库，最后对所有头文件和主函数，以及静态库执行
          g++ main.cpp -L./ -I./ -lxxx -o xxxx 生成名为xxxx的可执行文件
2.动态库： 先对所有的.cpp 文件执行 g++ -fPIC -c *.cpp 
          然后执行 g++ -shared *.o -o libxxx.so
          (也可以使用 g++ -shared -c *.cpp -o libxxx.so一步到位)
          使用方法同静态库，可以使用 nm libxxx.so来查看动态库中的内容，使用 nm libxxx.so | grep "yyy" 可以指定搜索
          //常见连接错误：
          ./a.out: error while loading shared libraries: libshared.so: cannot open shared object file: No such file or directory
          //一般可以把动态库添加到 lib/文件夹下， sourse生效
          //或者 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH: + 动态库的路径临时解决
          //或者在~/.bashrc 中添加 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH: + 动态库的路径临时解决
```
### GDB 调试
通过g++ -g 生成调试文件，gdb a.out 启动调试
```
run 开始调
start 向下一行
list / l + 数字或者函数名 可以显示源码， set listsize + 数字设置显示的行数

break / b + 数字或者函数名 可以设置断点， info / i + break / b 可以显示所有断点的信息

多文件可以通过作用域来添加断点 break filename:: + 数字或者函数名等

until 执行直到，可以用来跳出循环

ptype xxx查看数据

display 自动显示

disable + 命令可以禁用但是不删除
```
### Makefile
```c++
一条规则
    目标：依赖文件 // 可以有，也可以没有
      <TAB> 命令列表

    make -f 指定额外的输入文件
    make -w 显示进入和离开的目录
    make -C dir 进行make之前转到dir目录
    make -n 打印命令但是不执行，一般用于测试


例如：
test: add.cpp sub.cpp div.cpp mul.cpp test.cpp
	g++ add.cpp sub.cpp div.cpp mul.cpp test.cpp -o test


makefile 变量规则

    1 变量名 = 变量值

    2 引用变量：    
        $(变量名) 或者 ${变量名}

常用特殊变量：

    $@ 表示目标
    $^ 表示所有依赖
    $< 表示第一个依赖
    # 表示注释行
        //这些变量不能单独使用，必须在命令中使用

模式匹配：
    所有.o 依赖对应的 .c

    %.o:%.c

Makefile中的函数：
    1.wildcard 查找指定目录下指定类型的文件
        src = $(wildcard *.c) //找到当前目录下所有的.c文件并赋值给src
    2.patsubst 匹配替换
        obj = $(patsubst %.c,%.o,$(src)) //把src中所有.c替换为.o

声明伪目标（解决目录中有名为clean的文件导致make clean失效）
        .PHONY clean
//一个例子-------------------------------------------
----------------------------------------------------
XX = g++
SRC = $(wildward *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRC))
TARGET = test

$(TARGET): $(OBJS)
	@XX $^ -o $@
%.o: %.cpp
    @XX -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(OBJS) $(TARGET)
----------------------------------------------------
        

```

### 系统调用

系统调用：系统提供给用户的一种接口，属于操作系统的一部分，运行在内核态的进程有很高的权限，可以访问各种资源，而用户态的进程会有很多限制。

​	操作系统一般通过**软件中断**来实现用户态和内核态之间的切换。

库函数和系统调用：*库函数有需要系统调用和不需要系统调用两种*

### 虚拟地址空间

每一个进程都会分配虚拟地址空间，32位机器为4GB。

Linux 每一个运行的程序（进程）操作系统都会为其分配一个虚拟地址

**进程**  *正在运行的程序*

| 内核区                      | 命令行参数 |  栈空间  | 共享库     | 堆空间   | .bss           | .data  | 代码段 | 0 - 4k受保护的地址 |
| --------------------------- | ---------- | :------: | ---------- | -------- | -------------- | ------ | ------ | ------------------ |
| 错误访问会段错误**4GB-3GB** |            | 地址递减 | 存储映射区 | 地址递增 | 未初始化的数据 | 数据段 |        |                    |



**程序放在内存里面执行**

**MMU将虚拟地址转化为物理地址**（实现进程隔离，屏蔽物理差异带来的麻烦）



### 文件操作//系统调用

#### open() 和 write

```c++
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <iostream>

int main() {
    int fd = -1;
    //以只读的方式打开一个文件
    //fd = open("opentxt", O_RDONLY);
    //以只写的方式打开一个文件，存在就直接打开，没有就创建
    //fd = open("opentxt", O_WRONLY | O_CREAT, 0644);
    //以只写的方式打开，文件已经存在就报错，没有就创建
    //fd = open("opentxt", O_WRONLY | O_CREAT | O_EXCL, 0644);
    fd = open("opentxt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    std::cout << "fd = " << fd << std::endl;
    printf("fd = %d\n", fd);
    //关闭文件
    close(fd);
    return 0;
}
```
#### write() 和 read()

#### lseek() 偏移量
```c++
off_t lseek(int fd, off_t offset, int whence);
//文件打开有一个初始化偏移量cfo，为一个非负数
//whence: SEEK_SET 文件开头
            //SEEK_CUR 当前位置
            //SEEK_END 文件末尾
//执行成功返回新的偏移量，失败返回-1
```
#### stat 函数
```c++
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *path, struct stat *buf);
int lstat(cosnt char *pathname, struct stat *buf);

//获取文件状态信息 stat lstat 当文件是一个符号链接时，lstat返回符号链接本身的信息，stat返回指向的文件的信息
// path：文件名
//buf：保护文件信息结构体
//成功返回0， 失败返回 -1；
```

### 进程和程序
程序是静态的，进程是动态的，程序是储存在存储介质中的可执行文件，进程是程序执行的过程。
*进程是处理机分配资源的基本单元*
### MMU
内存管理单元，把虚拟地址映射到物理地址之上
### PCB
进程控制块，是进程存在的唯一标志。

进程 id :非负的整数

进程状态 ： 就绪，运行，挂起等状态

进程切换时需要保存和恢复的一些CPU寄存器

描述虚拟地址空间信息

文件描述符表

进程资源分配表

### 进程的状态
运行态，就绪态，阻塞态
创建态，终止态，运行态，就绪态，阻塞态
kill pid 结束进程 kill pid -9 强制结束

存在进程名字相同的进程，但是进程id一定不相同
killall 进程名字 -9 结束所有进程名字相同的进程

### 进程的创建
系统允许进程创建新的进程，新的进程就是子进程，形成进程树结构模型
```c++
#include <sys/types.h>
#include <unistd.h>

pid_t fork();
//功能：    用于从一个已经存在的进程中创建一个新的进程，新进程为子进程
//参数：    成功，子进程中返回0，父进程中返回子进程id
//            失败，返回 -1
//      失败的原因：当前进程数量已经达到系统规定的上限，errno的值为EAGAIN； 系统内存不足，errno值为ENOMEM
```
父子进程的关系：
子进程拥有和父进程相同的栈区，堆区，BSS区，数据段，代码段。（资源消耗比较大）
**写时拷贝，读时共享**

### 区分父子进程
**通过fork() 的返回值**

### 父子进程的地址空间

### 进程退出函数
```c++
#include <stdlib.h>
void exit(int status);  //可以刷新缓冲区

#include <unistd.h>
void _exit(int status); //属于系统调用，不会刷新缓冲区
```

### 等待子进程退出的函数
父进程可以通过调用wait 和 waitpid 得到它退出的状态，同时彻底关闭这个进程
wait() 和 waitpid() 的功能一样，区别在于，wait函数会阻塞，waitpid可以设置不阻塞，waitpid还可以指定等待哪个子进程结束
**wait和waitpid调用只能清理一个子进程，清理多个需要循环调用**
```c++
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
//函数会回收子进程的资源
```
wait()会挂起，直到一个子进程退出或者收到一个不能被忽视的信号才会被唤醒。
**wait()主要用于回收已经结束子进程的资源**

### 孤儿进程和僵尸进程
孤儿进程：父进程结束后，子进程还在运行的子进程为孤儿进程，每当出现孤儿进程，系统内核就把孤儿进程的父进程设置为**init**(一般为1号进程)，init进程会循环wait()它们已经退出的子进程。**孤儿进程没有严重的危害**
僵尸进程：进程终止，父进程未进行回收，子进程残留的资源(PCB)存放于内核之中。————导致的问题：如果进程不调用wait()或者waitpid()的话，那么保留的那段信息就不会被释放，其进程号会一直被占用，但是系统资源所能使用的继承号是有限的。

### 进程替换
exec函数族
```c++
#include <unistd.h>

extern char **environ;

int execl(const char *path, cosnt char *arg,...);
int execlp(......);
int execle(......);
int execv(......);
...
int execve(const char *filename, char *const argv[], char *const envp[]);
```
只有execve() 是真正意义上的系统调用，其他都是在此基础上包装的库函数。
**exec函数族的作用：根据指定的文件名或者目录名找到可执行文件，并用它来取代调用进程的内容**
***在调用进程内部执行一个可执行文件***
**前后pid相同**

### 进程间通信
进程是资源分配的最小单元，不能在一个进程中访问另一个进程的资源。但是，进程不是孤立的，不同的进程之间需要信息的交互，需要进程间通信(IPC)
**管道，信号，消息队列，共享内存，信号量，socket**

#### 匿名管道
1. 半双工
2. 数据只能从管道的一端写入，另一端读出
3. 写入管道的数据先进先出
4. 管道中传送的数据是无格式的，属于字节流，需要约定数据的格式
5. 管道不是普通的文件，只存在于内存中
6. 管道在内存中对应一个缓冲区，不同的系统大小不一定相同
7. 管道中的数据是一次性的，数据一旦被读走，就从管道中抛弃
8. 管道没有名字，只能在具有公共祖先的进程中使用
``` c++
#include <unistd.h>
int pipe(int pipefd[2]);
//创建匿名管道
//pipefd[2]用于存放读写的文件描述符。一般的IO函数都可以操作管道(lseek()除外);
//  成功返回 0， 失败返回 -1
```
### 查看管道大小
```c++
ulimit -a // 查看管道大小
```
#### 有名管道
命名管道FIFO文件，提供一个路径名与之关联，以FIFO文件的形式存在于文件系统中，这样，即使与FIFO创建进程没有亲缘关系，只要访问该路径，就可以彼此通过FIFO相互通信。
1. FIFO在文件系统中作为一个特殊的文件存在，但是内容存放在内存中。
2. 当使用FIFO的进程退出后，FIFO文件将继续保存在文件系统中以便以后使用。
3. FIFO有名字，不相关的进程可以通过打开命名管道进行通信
```c++
mkfifo fifo // 通过命令创建创建有名管道文件

// 通过函数创建
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
// pathname：创建后fifo的名字
//  mode: 文件的权限，与打开普通文件open()函数中的mode参数相同(0666);

```
**一个为只读而打开的一个管道进程会阻塞另一个为只写而打开的管道进程** **反过来一会造成阻塞**

### 共享存储映射(共享内存)
存储映射使一个磁盘文件与储存空间中的一个缓冲区相映射。
当从缓冲区中读数据，就相当于读文件中相应字节，将数据存入缓冲区，则相应的字节就自动写入文件。这样就可以在不使用read和write函数的情况下，使用地址指针完成I/O 操作

*进程不需要读写内存，不需要数据的拷贝*
**存储映射函数**
1. mmap 函数 建立映射
    **映射建立好后用 memcpy操作内存进行写入**
2. munmap 函数 释放内存映射

#### 使用匿名映射实现父子进程通信
**只适用于具有父子关系的进程**


### 信号
信号是实质上是一种软件中断。
1. 简单
2. 不能携带大量信息
3. 满足某个特定的时间才会发生
**信号可以直接进行用户空间进程和内核空间进程的交互**
**一个完整的信号周期：信号的产生，信号在进程中的注册与注销，执行信号处理函数。**
#### 信号的编号
POSIX标准
```c++
kill -l // 可以查看信号的编号
```
1-31 为常规信号，34-64 为实时信号

#### 信号四要素
1. 编号 2. 名称 3. 事件 4. 默认处理动作
**man 7 signal**查看

**SIGKILL 和 SIGSTOP 不能被捕获，阻塞和忽略**

#### 信号产生函数
kill 函数
```c++
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
//  给指定进程发送指定信号，不一定是kill
//  pid : 1. pid > 0 将信号发送给ID为 pid 的进程
//         2. pid = 0 将信号发送给进程组中的所有进程
//          3. pid = -1 将信号发送给系统中的所有进程
//            4. pid < -1 将信号传给指定的所有进程，进程组号为 pid 的绝对值

// sig 为信号的编号， 一般使用宏定义，可以通过
kill -l // 查看宏定义的内容。
// 成功 0       失败 -1
```
kill 受到权限的限制
