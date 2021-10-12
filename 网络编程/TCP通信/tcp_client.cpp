#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace std;

// TCP客户端
int main() {
    // 创建套接字
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    inet_pton(AF_INET, "192.168.1.104", &addr.sin_addr.s_addr);
    connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    // 读写数据
    char buf[1024] = "";
    while (1) {
        memset(buf, 0, sizeof buf);
        read(sock_fd, buf, sizeof (buf));
        cout << "buf: " << buf << endl;
        memset(buf, 0, sizeof buf);
        read(STDIN_FILENO, buf, sizeof buf);
        write(sock_fd, buf, sizeof buf);
        
    }
    close(sock_fd);
    return 0;
}