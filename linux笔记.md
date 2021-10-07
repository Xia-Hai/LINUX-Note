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

raise 函数
```c++
#include <signal.h>

int raise(int sig);
// 给当前进程发送指定信号(自己给自己发), 等价于kill(getpid(), sig)
//  参数 sig 为信号编号
```
abort() 函数

alarm 函数（闹钟）
```c++
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
// 设置定时器，在指定seconds后，内核会给当前进程发送 14 SIGALRM 信号，进程受到信号后，默认动作终止，每个进程都有且只有唯一一个定时器
// 取消定时器 alarm(0) ，返回旧闹钟余下的秒数
```

#### 信号注册和捕捉

### 信号集
阻塞信号集，未决信号集
内核使用**位图机制**来实现，但是不能直接对其进行位操作，需要自定义另一个集合，借助信号集操作函数来对PCB中的这两个信号集进行修改。
#### 信号集函数

#### sigaction 函数
```c++
#include <signal.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
// 检查或者修改指定信号的设置

//  参数：signum 要操作的信号
//      act 要设置的对信号新的处理方式
//      oldact 原来对信号的处理方式（传出参数）
```
### 不可重入， 可重入函数
一个函数：不同的任务调用这个函数可能修改其他任务调用这个函数的数据，从而导致不可预料的结果，这样的函数是不安全的函数，**也就是不可重入函数**
满足下列条件的函数大多数都是不可重入函数：
1. 函数体内使用静态数据结构
2. 函数体内调用了malloc() 或者 free()
3. 函数体内调用了标准I/O函数

相反，肯定有一个安全的函数，**可重入函数**
保证可重入函数的方法：
1. 在写函数的时候尽量使用局部变量
2. 要对使用的全局变量加以保护(信号量互斥，关中断) ，这样构成的函数就是一个可重入函数


#### SIGCHLD 信号
产生的条件：
1. 子进程终止时
2. 子进程收到SIGSTOP信号停止时
2. 子进程处于停止状态，接收到SIGCONT后唤醒

**避免僵尸进程**：

### 进程组
进程组，也就是作业，代表一个进程的集合。
每个进程都属于一个进程组，在waitpid函数和kill函数的参数中都被使用到。当父进程创建子进程，子进程默认和父进程属于同一个进程组。进程组ID为第一个进程的ID
### 会话
一个或多个进程组的集合（前台进程组只有一个）
getsid()  获取当前进程所属的会话ID
setsid()  创建一个会话，**不能再组长进程的基础上创建会话**

### 守护进程
脱离控制终端，生命周期长

### 线程
线程其实是一个轻量级进程，在linux环境下的本质仍然是进程，为了让进程完成工作，进程至少包含一个线程

**进程是CPU分配资源的最小单位，线程是CPU调度的基本单位** 线程是进程的实体，**线程本身不拥有资源，存在自己的栈空间，程序计数器，寄存器等**

#### 线程共享资源
1. 文件描述符表
2. 每种信号的处理方式
3. 当前工作目录
4. 用户ID和组ID
**内存地址空间**
#### 线程非共享资源
1. 线程ID 2. 处理器现场和栈指针(内核栈) 3. 独立的栈空间(用户栈) 4. errno 变量 5. 信号屏蔽字 6. 调度优先级

### 线程号
pthread_self() 函数。线程号只在所属的进程环境中有效
使用 phread_t 来表示

```c++
#include <pthread.h>

pthread_t pthread_self();
//   获取线程号
//   这个函数总是成功， 不会失败，不用判断返回值，类似于 umask()；
```
**需要指定link 线程库**

pthread_equal 函数
```c++
int pthread_equal(pthread_t t1, pthread_t t2);
// 相等为 非0， 不相等为0
```
**不能使用 == 来直接比较， 有些系统中pthread_t 定义是一个结构体，不能通过 == 比较**

### 线程的创建
pthread_creat 函数
```c++
#include <pthread.h>

int pthread_create(pthread_t *thread,
            const pthread_attr_t *attr,
            void *(*start_routine)(void *),
            void *arg);
    
// 创建一个线程
// thread：线程标识符地址  attr：线程属性结构体地址，通常为 NULL， start_routine：线程函数的入口地址， arg：传给线程函数的参数
// 成功返回 0
```
**在一个线程中调用pthread_creat 创建新的线程后，函数从返回处继续执行，新的线程所执行的代码由传入的函数指针 start_routine决定**
*pthread_creat 的错误码不在errno中，不能用perror打印，需要使用 strerror() 转换为错误码再打印*

### 线程资源回收
pthread_join 函数
```c++
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);

// 等待线程结束(此函数会阻塞), 回收线程资源，类似进程的wait() 函数，如果线程已经结束，立刻返回
// thread: 被等待的线程号
// retval: 用来储存线程退出状态的指针的地址
```
### 线程分离
一般情况下，线程终止后，其终止状态一直保留到其他线程调用pthread_join获取它为止，但是线程也可以被设置为 detach状态，这样的线程一旦终止就立刻回收它占用的所有资源，而不保留终止状态
**不能对一个使用pthread_join 的线程调用pthread_join,会返回 EINVAL 的错误**
```c++
#include <pthread.h>

int pthread_detach(pthread_t theard);
// 使调用线程和当前进程分离，分离后不代表改啊线程不依赖于该进程，线程分离的目的是将线程资源回收工作交给系统完成，也就是系统会自动回收它的资源。
```
**该函数不会被阻塞**

### 线程退出
1. 执行函数中 return 线程终止状态
2. pthread_exit(线程终止状态) 退出线程

### 线程取消
```c++
#include <pthread.h>

int pthread_cancel(pthread_t thread);

// 杀死(取消) 一个线程
```
**有一个延时，需要等待线程到达某个取消点** 类似于游戏必须在特定的地方存档


### 线程资源分配问题

##### 同步与互斥
互斥：临界资源同一时刻只能一个任务访问
同步：散布在不同任务之间的程序片断，它们的运行需要按照规定的某种先后次序来运行，这种次序依赖于要完成的特定任务。
##### 互斥锁的必要性
在多任务的操作系统中，同时运行的多个任务都需要使用同一种资源。避免交叉访问资源，出现错误

#### 互斥锁
在线程中存在互斥锁 mutex，也叫作 互斥量，互斥锁是一种简单的加锁方法来控制对共享资源的访问，互斥锁有两种状态，lock  和 unlock
**主要操作流程**
1. 在访问共享资源临界区域前，对互斥锁进行加锁
2. 访问完成后释放锁
3. 对互斥锁加锁后，任何其他试图再次对互斥锁加锁的线程将会被阻塞，直到锁被释放。

互斥锁的数据类型 pthread_mutex_t

pthread_mutex_init 函数
```c++
#include <pthread.h>

int pthread__mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
// 初始化一个互斥锁
// mutex: 互斥锁地址， attr：设置互斥锁属性，通常为默认属性NULL
// 可以使用宏 PTHREAD_MUTEX_INITIALiZER 静态初始化锁，等价于 attr 的参数 为NULL，但是不存在 错误检查
```
pthread_mutex_destroy 函数
```c++
#include <pthread.h>
int pthread_mutex_destroy(pthread_mutex_t *mutex);

// 销毁指定的互斥锁，互斥锁在使用完成后，必须要销毁释放资源
// mutex: address of the mutex
```
pthread_mutex_lock 函数
```c++
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex);
// 如果已经上锁会直接阻塞
int pthread_mutex_trylock(pthread_mutex_t *mutex);
// 调用该函数时，没有加锁就加锁，已经加锁就直接返回失败 ** EBUSY **
```
pthread_mutex_unlock 函数
```c++
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

#### 修改数据的不一致
多个线程共享数据段，同时访问内存，会出现修改数据不一致的情况，修改和写回内存不是原子操作
**通过上锁来实现**

#### 死锁的基本概念和场景
多个线程或者进程相互等待对方手中的资源而全部阻塞的情况。也就是申请资源的顺序不当。

原因：
1. 竞争不可抢占的资源
2. 竞争可以消耗的资源，进程或者线程推进的顺序不当

死锁的必要条件：
1. 互斥条件 ： 某个资源只能被一个线程或者进程使用，其他线程请求时，只能等待该线程释放资源
2. 请求和保持 ： 程序已经保持了至少一个资源，又提出新的要求，而这个资源被其他进程占用，自己的资源保持不放
3. 不可剥夺条件 ：资源不能被抢占，只能主动释放
4. 循环等待 ：存在一个循环链

处理死锁的思路：
预防死锁， 避免死锁， 检测死锁， 解除死锁

**破坏死锁必要条件之一来避免死锁**

### 读写锁
读多写少，使用互斥量效率就会非常低
多个线程同时读一个共享资源并不会造成影响
1. 有其他线程读数据，则允许其他线程执行读操作，不允许写操作
2. 有其他线程写数据，则不允许其他线程读 写操作

pthread_rwlock_init 函数
```c++
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
```
pthread_rwlock_destroy 函数
```c++
int pthread_rwlock_destroy(phtread_rwlock_t *rwlock);
```
phread_rwlock_rdlock 函数 pthread_rwlock_wrlock 函数加读锁和写锁

### 条件变量
条件变量本身不是锁，用来等待而不是上锁。
条件变量用来自动阻塞一个线程，直到某个特殊的情况发生为止。通常和互斥锁同时使用。
**条件变量的两个条件**
1. 条件不满足时，阻塞线程
2. 条件满足时，通知阻塞线程开始工作
----条件变量的类型为   pthread_cond_t

pthread_cond_init 函数
```c++
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
// 同之前的锁 基本一致
```
**pthread_cond_destroy 函数**
**pthread_cond_wait 函数**
```c++
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
// 阻塞等待某个条件变量
// 1 阻塞等待条件变量cond 满足
// 2 释放已经掌握的互斥锁，相当于pthread_mutex_unlock
// 1 2 为一个原子操作
// 3 当被唤醒，pthread_cond_wait函数返回时，解除阻塞并且重新申请互斥锁 pthread_mutex_lock
```
### 生产者消费者条件变量模型
线程同步典型案例  生产者消费者模型，而借助条件变量来实现这一模型是一种常见的方式。
假设有两个线程，一个模拟生产者行为，一个模拟消费者行为，两个线程同时操作一个共享资源，生产者向其中添加东西，消费者从中消费东西
**可以使用链表操作来模拟**

### 信号量
信号量广泛用于进程和线程间的同步和互斥，信号量本质上是一个非负的整数计数器，被用来控制对公共资源的访问。
**信号量** 大于0，可以访问，反之阻塞

PV原语 用于互斥和同步

sem_init 函数 和 sem_destroy 函数
```c++
#include <semaphore.h>
int sem_init(sem_t *sem, int pshared, unsigned int value);
// 创建一个信号量并初始化，一个无名的信号量必须在使用之前初始化
// pshared = 0 信号量在线程间共享， ！= 0，在进程间共享     value ： 信号量的初始值
int sem_destroy(sem_t *sem);
```
P操作
```c++
int sem_wait(sem_t *sem);
// 将信号量减 1 操作前检查sem是不是0，如果是 0 此函数会阻塞，直到信号量sem 大于0时才能减 1

int sem_trywait(sem_t *sem);
// 以非阻塞的方式来对信号量 减1 如果操作前 sem = 0， 则操作失败，直接返回
```
V操作
```c++
int sem_post(sem_t *sem);
// 将信号量加 1， 并发出信号唤醒等待的线程
```
获取信号量的值
```c++
#include <semaphore.h>
int sem_getvalue(sem_t *sem, int *sval);
// 获取信号量的值并保存在 sval 中
```

