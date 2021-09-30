#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;
//  父子进程使用匿名映射进行通行
int main() {
    int ret = -1;
    pid_t pid = -1;
    // 1 创建匿名映射
    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    // 2 创建子进程
    pid = fork();
    if (pid == -1) {
        perror("fork");
        munmap(addr, 4096);
        exit(1);
    }
    // 3 父子进程通信
    if (pid == 0) {
        // 子进程
        memcpy(addr,"1234567890", 10);
    } else {
        wait(NULL); //  等待子进程结束
        cout << "父进程 " << (char *)addr << endl;
    }
    // 4 断开映射
    munmap(addr, 4096);
    return 0;
}