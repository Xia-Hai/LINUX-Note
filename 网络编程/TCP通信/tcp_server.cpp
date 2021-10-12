#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    // 创建套接字
    int l_fd = socket(AF_INET, SOCK_STREAM, 0); // 用于监听的套接字
    // 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    //addr.sin_addr.s_addr = 0; // 0 表示绑定通配地址
    inet_pton(AF_INET, "192.168.1.104", &addr.sin_addr.s_addr);
    int ret = bind(l_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind");
        exit(1);
    }
    // 监听
    listen(l_fd, 128);
    // 提取
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int c_fd = accept(l_fd, (struct sockaddr *)&cliaddr, &len);
    char ip[16] = "";
    cout << "new client ip = " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16) << endl;
    cout << "new port = " << ntohs(cliaddr.sin_port) << endl;
    // 读写
    char buf[1024] = "";
    while (1) {
        memset(buf, 0, sizeof buf);
        read(STDIN_FILENO, buf, sizeof buf);
        write(c_fd, buf, sizeof buf);
        int ret = read(c_fd, buf, sizeof buf);
        if (ret == 0) {
            cout << "client close " << endl;
            break;
        }
        cout << "buf: " << buf << endl;
    }
    // 关闭
    close(l_fd);
    close(c_fd);
    return 0;
}