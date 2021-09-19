#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main() {
    pid_t pid = -1;
    pid = getpid();
    cout << "pid: " << endl;
    cout << "进程号：" << getpid() << endl;
    cout << "父进程号：" << getppid() << endl;
    return 0;
}