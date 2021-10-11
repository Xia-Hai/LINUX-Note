#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>


using namespace std;

// TCP客户端
int main() {
    // 创建套接字
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "111.111.111.111", &addr.sin_addr.s_addr);
    connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    // 读写数据
    
    return 0;
}