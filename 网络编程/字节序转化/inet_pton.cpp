#include <iostream>
#include <arpa/inet.h>
#include <string>

using namespace std;
// 点分十进制的转换
int main() {
    //char buf[] = "192.168.1.2";
    string buf = "192.168.1.1";
    unsigned int num = 0;
    inet_pton(AF_INET, buf.c_str(), &num);
    unsigned char *p = (unsigned char *)&num;
    cout << (int)*p << " " << (int)*(p + 1) << " "
        << (int)*(p + 2) << " " << (int)*(p + 3) << endl;
    char ip[16] = "";
    inet_ntop(AF_INET, &num, ip, 16);
    cout << ip << endl;
    return 0;
}