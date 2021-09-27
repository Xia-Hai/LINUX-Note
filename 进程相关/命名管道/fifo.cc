#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

// 通过函数创建有名管道
int main() {
    int ret = -1;
    ret = mkfifo("fifo", 0644);
    if (-1 == ret) {
        perror("fifo");
        return 1;
    }
    cout << "创建一个有名管道" << endl;
    return 0;
}