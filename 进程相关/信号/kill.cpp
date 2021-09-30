#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

// 父进程杀死子进程
int main() {
    pid_t pid = -1;
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // son
        while (1) {
            cout << "son is working" << endl;
            sleep(1);
        }
        exit(0);    // 进程退出
    } else {
        //  parent
        sleep(3);
        cout << "kill son" << endl;
        kill(pid, SIGTERM); // 15 SIGTERM 正常终止信号
        cout << "parent quit" << endl;
    }
    return 0;
}