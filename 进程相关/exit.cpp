#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

//结束进程
int main() {
    cout << "hello world";
    exit(0);//等价于return 0；
    //_exit(0);     //不会刷新缓冲区，强制退出
    //return 0;
}