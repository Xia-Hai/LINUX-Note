#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;


void func1(int signum) {
    cout << "捕捉到信号 ： " << signum << endl;
}
void func2(int signum) {
    cout << "捕捉到信号 ： " << signum << endl;
}

int main() {
    // 信号注册
    signal(SIGINT, func1);   // ctrl + c
    signal(SIGQUIT, func2);     // ctrl + /

    while (1) {
        getchar(); // 保证程序不退出
    }
    return 0;
}