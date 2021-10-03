#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void fun(int signum) {
    cout << "捕捉到信号 " << signum << endl;
    cout << "子进程需要回收" << endl;
}

int main() {
    pid_t pid = -1;
    struct sigaction act;
    act.sa_handler = fun;
    act.sa_flags = 0;
 
    // 注册信号处理函数
    sigaction(SIGCHLD, &act, NULL);
    // 创建一个子进程
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    }
    if (0 == pid) {
        // 子进程
        cout << "子进程睡眠 2 s" << endl;
        sleep(2);
        cout << "子进程退出" << endl;
        exit(0);
    }
    else {
        //  父进程
        while (1) {
            cout << "父进程作事" << endl;
            sleep(1);
        }
    }
    return 0;
}