#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

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
    //初始化pollfd数组
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int nfds = lfd;
    while (true) {
        // 调用poll系统函数
        int ret = poll(fds, nfds + 1, -1);
        if (ret == -1) {
           perror("poll");
           return -1; 
        }
        else if (ret == 0) {
            continue;
        }
        if (fds[0].revents & POLLIN) {
            sockaddr_in cliaddr;
            socklen_t len = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
            if (cfd == -1) {
                perror("accpet");
                return -2;
            }
            // 输出连接信息
            char ip[16] = {0};
            unsigned short port = 0;
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16);
            port = ntohs(cliaddr.sin_port);
            cout << "IP: " << ip << " Port: " << port << endl;
            // cfd 加入pollfd中
            int i;
            for (i = 1; i < 1024; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = cfd;
                    fds[i].events = POLLIN;
                    break;
                }      
            }
            if (i == 1024) {
                perror("poll error");
                return 1; // 超过了最大容量
            }
            nfds = nfds > i ? nfds : i;
        }
        for (int i = 1; i <= nfds; i++) {
            if (fds[i].fd < 0) continue;
            if (fds[i].revents & POLLIN) {
                char buf[1024] = {0};
                int len = read(fds[i].fd, buf, sizeof(buf));
                if (len == 0) {
                    cout << "client closed..." << endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    continue;
                }
                else if (len == -1) {
                    perror("read");
                    return -3;
                }
                //cout << "rev buf: " << buf << endl;
                write(STDOUT_FILENO, buf, len);
                cout << endl;
                write(fds[i].fd, buf, strlen(buf) + 1);
            }  
        }
    }
    close(lfd);
    return 0;
}