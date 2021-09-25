#include <iostream>
#include <unistd.h>
#include <errno.h>

using namespace std;

//孤儿进程demo
int main() {
    pid_t pid = -1;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid > 0) {
        //父进程
        cout << "父进程睡眠1s" << endl;
        sleep(1);
        cout << "父进程退出" << endl;
        exit(0);
    }
    //子进程
    while (1) {
        cout << "子进程持续工作 ppid = " << getppid() << endl;
        sleep(1);
    }
    return 0;
}