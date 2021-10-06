#include <iostream>
#include <pthread.h>
#include <unistd.h> // sleep
using namespace std;

// 条件变量的使用
int flag = 0;
// 互斥量
pthread_mutex_t mutex;
// 条件变量
pthread_cond_t cond;
// 1. 改变条件的线程
void *fun1(void *arg) {
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        flag = 1;
        // 解锁
        pthread_mutex_unlock(&mutex);

        // 唤醒因为条件而阻塞的线程
        pthread_cond_signal(&cond);

        sleep(2);
    }
    return NULL;
}
// 2. 等待条件的线程
void *fun2(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        // 表示条件不满足
        if (flag == 0) {
            // 等待条件满足
            pthread_cond_wait(&cond, &mutex);
        }
        cout << "条件满足, 开始运行..." << endl;
        flag = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    int ret = -1;

    // 初始化互斥量和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_t tid1, tid2;
    // 创建两个线程
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // 回收线程资源
    ret = pthread_join(tid1, NULL);
    if (ret != 0) {
        cout << "pthread_join failed..." << endl;
        exit(1);
    }
    ret = pthread_join(tid2, NULL);
    if (ret != 0) {
        cout << "pthread_join failed..." << endl;
        exit(1);
    }
    // 销毁互斥量和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}