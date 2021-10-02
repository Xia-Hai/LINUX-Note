#include <unistd.h>
#include <iostream>
#include <signal.h>

using namespace std;

// 自己给自己发送信号

int main() {
    int i = 1;
    while (1) {
        cout << "do something here" << endl;
        if (i == 4) {
            // 自己给自己发送信号
            raise(SIGTERM); // 15 SIGTERM
        }
        i++;
        sleep(1);
    }
    return 0;
}
        