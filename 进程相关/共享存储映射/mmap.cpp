#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>

using namespace std;
//存储映射
int main() {
    int fd = -1;
    int ret = -1;
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

    memcpy(addr, "1234567890", 10);
    //断开映射
    munmap(addr, size);
    return 0;
}