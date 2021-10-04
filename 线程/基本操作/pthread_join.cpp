#include <iostream>
#include <pthread.h>
#include <unistd.h> // sleep

using namespace std;

// 回收线程资源
void *fun(void *arg) {
    for (int i = 0; i < 5; i++) {
        cout << "i = " << i << endl;
        sleep(1);
    }
    return (void *)0x3;
}

int main() {
    int ret = -1;
    void *retp = nullptr;
    pthread_t tid = -1;
    // creat a thread
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (ret != 0) {
        cout << "pthread_creat failed..." << endl;
        exit(1);
    }
    cout << "main thread run... " << endl;
    // 等待线程结束
    ret = pthread_join(tid, &retp);
    if (ret) {
        cout << "pthread_join failed..." << endl;
        exit(1);
    }
    cout << "retp: " << retp << endl;
    cout << "main thread exit..." << endl;
    return 0;
}