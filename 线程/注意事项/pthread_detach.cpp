#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// pthread_detach 使用
void *fun(void *arg) {
    cout << "new thread created..." << endl;
    for (int i = 0; i < 5; i++) {
        cout << "new thread does something..." << i << endl;
        sleep(1);
    }
    return nullptr;
}

int main() {
    int ret = -1;
    pthread_t tid = -1;
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (ret != 0) {
        cout << "pthread_create failed..." << endl;
        exit(1);
    }
    
    // 设置线程分离
    ret = pthread_detach(tid);
    if (ret != 0) {
        cout << "pthread_detach failed..." << endl;
        exit(1);
    }
    cout << "press any key to exit" << endl;
    getchar();
    return 0;
}