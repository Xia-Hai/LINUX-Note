#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

//等待子进程退出
int main() {
    int status = 0;
    int ret = 0;
    pid_t pid = -1;
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        for (int i = 0; i < 5; i++) {
            cout << "child process do " << i <<endl;
            sleep(1);
        }
        exit(1);
    }
    else {
        cout << "父进程等待子进程退出，回收资源" << endl;
        ret = wait(&status);
        if (ret == -1) {
            perror("wait");
            return 1;
        }
        cout << "父进程回收子进程的资源" << endl;
    }
    return 0;
}