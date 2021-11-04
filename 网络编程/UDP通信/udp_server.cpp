#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

int main() {
    
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        return -2;
    }

    while (true) {

        char buf[128] = {0};
        char ip[16] = {0};
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int num = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16);
        unsigned short port = ntohs(cliaddr.sin_port);
        cout << "IP: " << ip << " Port: " << port << endl;
        cout << "rev: " << buf << endl;
        sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    }
    close(fd);
    return 0;
}