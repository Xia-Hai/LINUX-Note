#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

using namespace std;

int main() {
    
    // 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    // listen
    listen(lfd, 8);
    
    // 创建一个fd_set 的集合
    fd_set rdset, temp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;
    while (1) {
        temp = rdset;
        // 调用select系统函数
        int ret = select(maxfd + 1, &temp, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            exit(1);
        }
        else if (ret == 0) {
            continue;
        }
        else { // ret > 0
            // 说明检测到文件描述符发生改变
            if (FD_ISSET(lfd, &temp)) {
                // 表示有客户端连接进来
                struct sockaddr_in cliaddr;
                socklen_t len = (socklen_t)sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
                char ip[16] = {0};
                inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip));
                unsigned short port = ntohs(cliaddr.sin_port);
                cout << "IP: " << ip << " " << "Port: " << port << endl;
                // 将新的文件描述符加入集合中
                FD_SET(cfd, &rdset);
                // 更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            
            for (int i = lfd + 1; i <= maxfd + 1; i++) {
                if (FD_ISSET(i, &temp)) {
                    // 说明对应的文件描述符有数据
                    char buf[1024] = {0};
                    int len = read(i, buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(1);
                    }
                    else if (len == 0) {
                        cout << "client closed..." << endl;
                        close(i);
                        FD_CLR(i, &rdset);
                    }
                    else {
                        cout << "read buf: " << buf << endl;
                        write(i, buf, strlen(buf) + 1);
                    }

                }
            }
        }
    }
    close(lfd);
    return 0;
}