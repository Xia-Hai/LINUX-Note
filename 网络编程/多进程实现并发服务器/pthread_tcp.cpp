#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>



using namespace std;
// 多线程实现TCP服务器

struct sockInfo {
    int fd; // 通信的文件描述符
    pthread_t tid; // 线程id
    struct sockaddr_in addr;
};

struct sockInfo sockinfos[128];

void *working(void *arg) {
    // 子线程通信 cfd, 客户端的信息，通过arg传入

    // 获取客户端的信息
    struct sockInfo *pinfo = (struct sockInfo *)arg;
    char cliIP[16];
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIP, sizeof(cliIP));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    cout << "client ip is: " << cliIP << " port is: " << cliPort << endl;
    char recvBuf[1024] = {0};
    while (1)
    {
        int len = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (len == -1)
        {
            perror("read");
            exit(1);
        }
        else if (len == 0)
        {
            cout << "client closed... " << endl;
            break;
        }
        else
        {
            cout << "recv data is: " << recvBuf << endl;
        }

        write(pinfo->fd, recvBuf, strlen(recvBuf) + 1);
    }
    return NULL;
}
int main() {
    

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

    // 初始化数据
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < max; i++) {
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    // 循环等待客户端的连接
    while (1) {

        struct sockaddr_in cliaddr;
        socklen_t len = (socklen_t)sizeof(cliaddr);
        // 接收连接
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

        struct sockInfo *pinfo;
        for (int i = 0; i < max; i++) {
            if (sockinfos[i].fd == -1) {
                pinfo = &sockinfos[i];
                break;
            }
            if (i == max - 1) {
                sleep(1);
                i = -1; // 重新开始循环
            }
        }
        pinfo->fd = cfd;
        memcpy(&pinfo->addr, &cliaddr, len);
        // 创建子线程
        
        pthread_create(&pinfo->tid, NULL, working, pinfo);

        // 不能用阻塞的线程回收
        pthread_detach(pinfo->tid);
    }
    close(lfd);
    return 0;
}