#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include <signal.h>
#include <sys/types.h>
#include "http_conn.h"

#define MAX_FD 65535 // 最大文件描述符的个数
#define MAX_EVENT_NUM 10000 // 最大支持事件的个数

// 添加信号捕捉
void addsig(int sig, void(*handler)(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig, &sa, NULL);
}

// 添加文件描述符到 epoll 中
extern void addfd(int epollfd, int fd, bool one_shot);

// 从 epoll 中删除
extern void removefd(int epollfd, int fd);

// 修改文件描述符
extern void modfd(int epollfd, int fd, int ev);


int main(int argc, char *argv[]) {

    if (argc <= 1) {
        std::cout << "按照如下格式运行：" << basename(argv[0]) << " port number" << std::endl;
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);

    // 对sigpie信号进行处理
    addsig(SIGPIPE, SIG_IGN);
    
    // 创建线程池，初始化线程池
    threadpool<http_conn> *pool = NULL;
    try {
        pool = new threadpool<http_conn>;
    } catch(...) {
        exit(-1); 
    }

    // 创建一个数组保存所有客户端的信息
    http_conn *users = new http_conn[MAX_FD];

    // 创建监听的文件描述符
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(listenfd, (struct sockaddr *)&address, sizeof(address));

    // 监听
    listen(listenfd, 5);

    // 创建epoll对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUM];
    int epollfd = epoll_create(5);

    // 将监听的文件描述符添加到epoll对象中
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    while (true) {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);
        if (num < 0 && errno != EINTR) {
            std::cout << "epoll failed..." << std::endl;
            break;
        }

        // 循环遍历epoll事件数组
        for (int i = 0; i < num; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_len);

                if (http_conn::m_user_count >= MAX_FD) {
                    // 连接满了
                    // 会给客户端一个信息
                    close(connfd);
                    continue;
                }
                // 将新的客户端数据初始化，放到数组中
                users[connfd].init(connfd, client_address);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                // 对方异常断开
                users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN) {
                if (users[sockfd].read()) {
                    // 一次性把数据都读出来
                    pool->append(users + sockfd);        
                } else {
                    users[sockfd].close_conn();
                }
            }
            else if (events[i].events & EPOLLOUT) {
                if (!users[sockfd].write()) {
                    // 一次性写完所有数据
                    users[sockfd].close_conn();
                }
            }
        }
    }
    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;

    return 0;
}