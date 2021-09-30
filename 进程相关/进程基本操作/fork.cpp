#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

//创建一个子进程
int main() {
    fork();
    cout << "hello hello" << endl;//输出两次
    return 0;
}