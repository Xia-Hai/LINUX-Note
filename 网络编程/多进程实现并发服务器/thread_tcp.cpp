#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wait.h>
#include <errno.h>

using namespace std;
// 多进程版本

void recycleChild(int arg) {
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) {
            // 子进程都回收完了
            break;
        }
        else if (ret == 0) {
            // 还有子进程在运行
            break;
        }
        else if (ret > 0) {
            // 被回收了
            cout << "子进程 " << ret << " 被回收了" << endl;
        }
    }
}

int main() {

    // 注册信号捕捉用来回收子进程资源
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    sigaction(SIGCHLD, &act, NULL);

    // 1. 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
       perror("socket");
       exit(1); 
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY; // 指定任意端口
    // 2. bind
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(1);
    }
    // 3. 监听
    ret = listen(lfd, 128);
    if (ret == -1) {
        perror("listen");
        exit(1);
    }

    // 循环等待客户端
    while (1) {

        struct sockaddr_in cliaddr;
        socklen_t len = (socklen_t)sizeof(cliaddr);
        // 接收连接
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
        if (cfd == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            exit(1);
        }

        // 每一个连接一个子进程来通信
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            // 获取客户端的信息
            char cliIP[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIP, sizeof(cliIP));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            cout << "client ip is: " << cliIP << " port is: " << cliPort << endl;
            char recvBuf[1024] = {0};
            while (1) {
                int len = read(cfd, &recvBuf, sizeof(recvBuf));
                if (len == -1) {
                    perror("read");
                    exit(1);
                }
                else if (len == 0) {
                    cout << "client closed... " << endl;
                    break;
                }
                else {
                    cout << "recv data is: " << recvBuf << endl;
                }

                write(cfd, recvBuf, strlen(recvBuf) + 1);
            }
            close(cfd);
            exit(0); // 子进程退出
        }
        
    }
    close(lfd);

    return 0;
}