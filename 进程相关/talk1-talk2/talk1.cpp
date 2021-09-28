#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <unistd.h> // write
#include <cstring> // memset

using namespace std;
//先读后写， 以只读打开fifo1， 以只写打开 fifo2

#define SIZE 128
int main() {
    int fdr = -1;   // 读
    int fdw = -1;   // 写
    int ret = -1;
    char buf[SIZE] = {0};

    //  只读打开FIFO1
    fdr = open("fifo1", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        return 1;
    }
    cout << "以只读的方式打开fifo1" << endl;

    //  以只写打开fifo2
    fdw = open("fifo2", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        return 1;
    }
    cout << "以只写的方式打开fifo2" << endl;

    // 循环读写
    while (1) {
        memset(buf, 0, SIZE);
        ret = read(fdr, buf, SIZE); // read fifo1
        if (ret <= 0) {
            perror("read");
            break;
        }
        cout << "read: " << buf << endl;

        // write fifo2
        memset(buf, 0, SIZE);
        // fgets(buf, SIZE, stdin);
        // if ('\n' == buf[strlen(buf) - 1]) {
        //     buf[strlen(buf) - 1] = '\0'; // 去掉换行符
        // }
        cin.getline(buf, SIZE);
        ret = write(fdw, buf, strlen(buf)); // write fifo2
        if (ret <= 0) {
            perror("write");
            break;
        }
        cout << "write ret: " << ret << endl;
    }

    close(fdr);
    close(fdw);
    return 0;
}