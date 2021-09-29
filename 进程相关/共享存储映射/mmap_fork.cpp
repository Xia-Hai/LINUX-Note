#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;
//存储映射
int main() {
    int fd = -1;
    int ret = -1;
    pid_t pid = -1;
    fd = open("1.txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        exit(1);
    }
    int size = lseek(fd, 0, SEEK_END);
    // 建立映射
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    cout << "建立文件存储映射" << endl;
    close(fd);

    //  创建子进程
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // 子进程 写文件
        memcpy(addr, "abcdefghijk", 11);
    } else {
        // 父进程 读文件
        // 等待子进程结束
        wait(NULL);
        cout << "addr : " << (char *)addr << endl;
    }



    
    //断开映射
    munmap(addr, size); //父子进程都会执行这一步
    return 0;
}