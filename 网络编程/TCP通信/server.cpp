#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace std;

// TCP通信服务端简单实现

int main() {
    // 1. 创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(1);
    }
    // 2. 绑定端口号和ip地址
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.1.104", &saddr.sin_addr.s_addr);
    // saddr.sin_addr.s_addr = 0; // 0.0.0.0 指定所有网卡对应的ip
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    // 3. 监听
    ret = listen(lfd, 8);
    if (ret == -1) {
        perror("listen");
        exit(1);
    }

    // 4. 接收客户端的连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    if (cfd == -1) {
        perror("accept");
        exit(1);
    }
    char clientIP[16] = {0};
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short int clientPort = ntohs(clientaddr.sin_port);
    cout << "client IP is: " << clientIP << endl;
    cout << "client port is: " << clientPort << endl;
    // 5. 收发数据
    char recvBuf[1024] = {0};
    char data[1024] = {0};
    while(1) {
        memset(recvBuf, 0, sizeof(recvBuf));
        int size = read(cfd, recvBuf, sizeof(recvBuf));
        if (size == -1) {
            perror("read");
            exit(1);
        }
        else if (size > 0) {
            cout << "recv Client data: " << recvBuf << endl;
        }
        else {
            cout << "client shutdown the connect..." << endl;
        }
        memset(data, 0, sizeof(data));
        read(STDIN_FILENO, data, sizeof(data));
        ret = write(cfd, data, sizeof(data));
        if (ret == -1) {
            perror("write");
            exit(1);
        }
    }
    // 6. 关闭
    close(lfd);
    close(cfd);
    return 0;
}