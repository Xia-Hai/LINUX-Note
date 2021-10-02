#include <unistd.h>
#include <iostream>

using namespace std;

#define UI unsigned int

//alarm
int main() {
    UI ret = 0;
    // 第一次设置闹钟 5 秒后超时， 发送对应的信号

    ret = alarm(5);
    cout << "上一次闹钟剩下的时间" << ret << endl;
    sleep(2);
    // 之前没有超时的闹钟被新的设置覆盖
    ret = alarm(4);
    cout << "上一次闹钟剩下的时间" << ret << endl;
    cout << "按下任意键继续" << endl;
    getchar();
    return 0;
}