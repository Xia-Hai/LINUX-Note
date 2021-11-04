#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
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
    // 调用epoll_creat() 创建一个epoll实例
    int epfd = epoll_create(100);
    // lfd 相关信息加入红黑树
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);
    
    struct epoll_event epevs[1024];

    while (true) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll");
            return -1;
        }
        cout << "ret = " << ret << endl; // 改变的个数
        for (int i = 0; i < ret; i++) {
            int curfd = epevs[i].data.fd;
            if (curfd == lfd) {
            // 监听的文件描述符有数据到达
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
     
            epev.events = EPOLLIN;
            epev.data.fd = cfd;
            epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }
            else {
                if (epevs[i].events & EPOLLOUT) {
                    // do write
                    continue;
                }
                // else do read
                char buf[5] = {0};
                int len = read(curfd, buf, sizeof(buf));
                if (len == 0) {
                    cout << "client closed..." << endl;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                    continue;
                }
                else if (len == -1) {
                    perror("read");
                    return -3;
                }
                //cout << "rev buf: " << buf << endl;
                //cout << strlen(buf) << endl;
                buf[5] = '\0';
                write(STDOUT_FILENO, buf, len);
                cout << endl;
                write(curfd, buf, strlen(buf) + 1);   
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}