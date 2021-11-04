#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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
            //ip[strlen(ip) + 1] = '\0';
            port = ntohs(cliaddr.sin_port);
            cout << "IP: " << ip << " Port: " << port << endl;
            
            // 设置 cfd 为非阻塞
            int flag = fcntl(cfd, F_GETFL);
            flag |= O_NONBLOCK;
            fcntl(cfd, F_SETFL, flag);

            epev.events = EPOLLIN | EPOLLET;
            epev.data.fd = cfd;
            epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }
            else {
                
               
                // 循环读取数据 
                char revbuf[5];
                memset(revbuf, 0, sizeof(revbuf));
                int len = 0;
                while ((len = read(curfd, revbuf, sizeof(revbuf))) > 0) {
                    //buf[5] = '\0';
                    //cout << len  << endl;
                    //cout << "rev data: " << revbuf << endl;
                    write(STDOUT_FILENO, revbuf, len);
                    write(curfd, revbuf, len);
                    memset(revbuf, 0, sizeof(revbuf));   
                }
                if (len == 0) {
                    cout << "client closed..." << endl;
                }
                else if (len == -1) {
                    if (errno == EAGAIN) {
                        cout << "data over..." << endl;
                    }
                    else {
                        perror("read");
                        return -3;
                    }
                } 
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}