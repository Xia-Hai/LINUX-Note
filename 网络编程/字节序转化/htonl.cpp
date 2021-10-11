#include <iostream>
#include <cstdio>
#include <arpa/inet.h>

using namespace std;

int main() {
    unsigned char buf[4] = {192, 168, 1, 100};
    int num = *(int *)buf;
    int sum = htonl(num);
    unsigned char *p = (unsigned char *)&sum;
    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));
   
    printf("====================\n");
    unsigned short a = 0x0102;
    unsigned short b = htons(a);
    printf("%x\n", b);
    cout << "===================" << endl;
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf1;
    int sum1= ntohl(num1);
    unsigned char *p1 = (unsigned char *)&sum1;
    printf("%d %d %d %d\n", *p1, *(p1 + 1), *(p1 + 2), *(p1 + 3));
    return 0;
}