#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>

using namespace std;
constexpr int SIZE = 128;

int main() {
    //1 open file
    int fd = -1;
    int ret = -1;
    char buf[SIZE] = {0};
    //string buf;
    fd = open("txt", O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    cout << "fd = " << fd << endl;
    //2 lseek
    write(fd, "ABCDEFG", 7);
    ret = lseek(fd, 32, SEEK_SET);
    if (-1 == ret) {
        perror("lseek");
        return 1;
    }
    write(fd, "1223455555", 10);

    lseek(fd, 0, SEEK_SET);
    ret = read(fd, buf, SIZE);
    //ret = read(fd, buf, SIZE);
    printf("buf = %s ret = %d\n", buf, ret);
    cout << "buf = " << buf << endl;
    //3 close
    close(fd);
    return 0;
}