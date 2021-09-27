#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <unistd.h> // write
#include <cstring> // memset

using namespace std;

// 写入有名管道有名管道
#define SIZE 128
int main() {
    int fd = -1;
    int i = 1;
    char buf[SIZE] = {0};
    int ret = -1;
    // 1.写方式打开一个管道文件
    fd = open("fifo", O_WRONLY);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    cout << "以只写的方式打开一个管道" << endl;
    // 2. 写管道
    while (1) {
        memset(buf, 0, SIZE);
        snprintf(buf, 20, "%08d %s", i++, "xiahai");
        //sprintf(buf, "xiahai %d", i++);
        ret = write(fd, buf, strlen(buf));
        if (ret <= 0) {
            perror("write");
            break;
        }
        cout << "write fifo: " << ret << endl;
        sleep(1);
    }
    // 3. 关闭文件
    close(fd);
    return 0;
}