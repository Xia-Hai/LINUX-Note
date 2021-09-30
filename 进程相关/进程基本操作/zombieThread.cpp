#include <iostream>
#include <unistd.h>
 
using namespace std;

//创建僵尸进程
int main() {
    pid_t pid = -1;
    //创建一个子进程
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    //子进程
    if (pid == 0) {
        for (int i = 0; i < 5; i++) {
            cout << "子进程工作" << endl;
            sleep(1);
        }
        cout << "子进程自己结束" << endl;

        //子进程退出
        exit(0);
    }
    sleep(50);
    cout << "父进程退出……" << endl;
    return 0;
}   