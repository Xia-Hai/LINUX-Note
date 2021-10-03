#include <iostream>
#include <signal.h>

using namespace std;
//  sigaction 函数的使用


// 信号处理函数
void fun(int signum) {
    cout << "捕捉到信号： " << signum << endl;
}

void fun1(int signum, siginfo_t *info, void *context) {
    cout << "捕捉到信号 " << endl;
}
int main() {
    int ret = -1;
#if 0
    struct sigaction act;
    act.sa_handler = fun;
    act.sa_flags = 0;
#else
    // 注册
    struct sigaction act;
    act.sa_sigaction = fun1;
    act.sa_flags = SA_SIGINFO;
#endif
    ret = sigaction(SIGINT, &act, NULL);
    if (-1 == ret) {
        perror("sigaction");
        exit(1);
    }
    cout << "按下任意键继续" << endl;
    while (1) {
        getchar();
    }
    return 0;
}