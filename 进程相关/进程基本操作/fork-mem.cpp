#include <iostream>
#include <cstring>
#include <unistd.h>

using std::cout;
using std::endl;

//全局变量
int num = 100;

//父子进程的地址空间
int main() {
    int a = 10;
    //堆区的数据
    int *p = new int;
    *p = 200;
    pid_t pid = -1;
    //创建一个子进程
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        //子进程
        sleep(1);
        cout << "子进程睡眠之后 a 的值: " << a << endl;
        cout << "子进程睡眠之后 num 的值: " << num << endl;
        cout << "子进程睡眠之后 *p 的值: " << *p << endl;
        delete p;
    }
    else {
        //父进程
        cout << "父进程之前 a 的值：" << a << endl;
        cout << "父进程之前 num 的值：" << num << endl;
        cout << "父进程之前 *p 的值：" << *p << endl;
        a++;
        num++;
        (*p)++;
        cout << "父进程之后 a 的值：" << a << endl;
        cout << "父进程之后 num 的值：" << num << endl;
        cout << "父进程之后 *p 的值：" << *p << endl;
        delete p;
    }
    return 0;
}