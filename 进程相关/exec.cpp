#include <iostream>
#include <unistd.h>

using namespace std;

//进程替换
//int execlp(const char *file, const char *arg, ...);
int main() {
    cout << "这是当前进程" << endl;
    //arg[0] arg[1] arg[2]...
    //arg[0]为可执行文件的名字，后面接传入的参数，arg[n]为NULL
    //等价于 ls -l /home
    execlp("ls", "ls", "-l","/home", NULL);
    cout << "还在吗" << endl;
    return 0;
}