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



