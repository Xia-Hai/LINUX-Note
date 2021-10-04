#include <iostream>
#include <pthread.h>
#include <unistd.h> // usleep

using namespace std;
// 模拟输出字符

void *fun1(void *arg) {
    // 输出大写字母
    for (char i = 'A'; i <= 'Z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000); // 100ms
    }
    cout << endl;
    return NULL;
}

void *fun2(void *arg) {
    // 输出小写字母
    for (char i = 'a'; i <= 'z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000);
    }
    cout << endl;
    return NULL;
}
// 两个线程一定交替进行
int main() {
    pthread_t tid1, tid2;

    // 创建两个线程
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL); // 最好加上判定函数是否成功执行
    pthread_join(tid2, NULL);

    cout << "main thread exit..." << endl;
    return 0;
}