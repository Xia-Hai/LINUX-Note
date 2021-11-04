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
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.1.104", &saddr.sin_addr.s_addr);

    
    int num = 0;
    while (true) {

        char buf[128] = {0};
        sprintf(buf, "hello, i am %d\n", num++);
        sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&saddr, sizeof(saddr));

        int num = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
       
        cout << "rev: " << buf << endl;
        sleep(1);
    }
    close(fd);
    return 0;
}