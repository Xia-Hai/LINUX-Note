#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace std;
// TCP通信客户端简单实现

int main() {
    // 1. 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(1);
    }
    
    // 2. 连接服务器
    struct sockaddr_in serveraddr;
    inet_pton(AF_INET, "192.168.1.104", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1) {
        perror("connect");
        exit(1);
    }

    // 3. 通信
    //char data[1024] = {0};
    char recvBuf[1024] = {0};
    int i = 0;
    while (1) {
        //memset(data, 0, sizeof(data));
        sprintf(recvBuf, "data : %d", i++);
        
        //read(STDIN_FILENO, data, sizeof(data));

        ret = write(fd, recvBuf, strlen(recvBuf) + 1);
        //i++;
        //sleep(1);
        if (ret == -1) {
            perror("write");
            exit(1);
        }

        memset(recvBuf, 0, sizeof(recvBuf));
        int size = read(fd, recvBuf, sizeof(recvBuf));
        if (size == -1) {
            perror("read");
            exit(1);
        }
        else if (size > 0) {
            cout << "recv Server data: " << recvBuf << endl;
        }
        else {
            cout << "server shutdown the connect..." << endl;
        }
        sleep(1);
        
    }
    // 关闭
    close(fd);
    return 0;
}