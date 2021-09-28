#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <unistd.h> // write
#include <cstring> // memset

using namespace std;


#define SIZE 128
int main() {
    int fdr = -1;   // 读
    int fdw = -1;   // 写
    int ret = -1;
    char buf[SIZE] = {0};

    
    fdw = open("fifo1", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        return 1;
    }
    cout << "以只写的方式打开fifo1" << endl;

   
    fdr = open("fifo2", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        return 1;
    }
    cout << "以只读的方式打开fifo2" << endl;

    // 循环读写
    while (1) {

         // write fifo1
        memset(buf, 0, SIZE);
        // fgets(buf, SIZE, stdin);
        // if ('\n' == buf[strlen(buf) - 1]) {
        //     buf[strlen(buf) - 1] = '\0'; // 去掉换行符
        // }
        cin.getline(buf, SIZE);
       

        ret = write(fdw, buf, strlen(buf)); // write fifo1
        if (ret <= 0) {
            perror("write");
            break;
        }
        cout << "write ret: " << ret << endl;

        memset(buf, 0, SIZE);
        ret = read(fdr, buf, SIZE); // read fifo2
        if (ret <= 0) {
            perror("read");
            break;
        }
        cout << "read: " << buf << endl;

       
    }

    close(fdr);
    close(fdw);
    return 0;
}