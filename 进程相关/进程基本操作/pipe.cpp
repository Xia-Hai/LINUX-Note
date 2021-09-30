#include <iostream>
#include <unistd.h>

using namespace std;

// //  创建匿名管道
// int main() {
//     int fd[2];
//     int ret = -1;
//     ret = pipe(fd);
//     if (-1 == ret) {
//         perror("pipe");
//         return 1;
//     }
//     // fd[0]用于读； fd[1] 用于写
//     cout << "fd[0] = " << fd[0] << " " << "fd[1] = " << fd[1] << endl;
//     close(fd[0]);
//     close(fd[1]);
//     return 0;
// }

//父子进程使用匿名管道进行通信
//父进程写，子进程读

const int SIZE = 64;
int main()
{
    pid_t pid = -1;
    int fds[2] = {0};
    char buf[SIZE] = {0};
    int ret = -1;
    ret = pipe(fds);
    if (-1 == ret) {
        perror("pipe");
        return 1;
    }
    //创建子进程
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        //子进程
        close(fds[1]);  //关闭写端
        ret = read(fds[0], buf, SIZE);
        if (ret == -1) {
            perror("read");
            exit(-1);
        }
        cout << "子进程读到的buf: " << buf << endl;

        close(fds[0]);
        exit(0);
    }
    // 父进程
    close(fds[0]);
    ret = write(fds[1], "ABCDEFG", 7);
    if (ret < 0) {
        perror("write");
        return 1;
    }
    cout << "父进程写入的长度为：" << ret << endl;
    close(fds[1]);
    return 0;
}