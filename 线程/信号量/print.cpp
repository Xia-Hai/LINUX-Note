#include <iostream>
#include <pthread.h>
#include <unistd.h> // usleep
#include <semaphore.h>

using namespace std;
// 模拟输出字符

// 信号量变量
sem_t sem;

void *fun1(void *arg) {
    // 输出大写字母
    // 申请资源 P 操作
    sem_wait(&sem);
    for (char i = 'A'; i <= 'Z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000); // 100ms
    }
    cout << endl;
    // 释放资源 V 操作
    sem_post(&sem);
    return NULL;
}

void *fun2(void *arg) {
    // 输出小写字母
    sem_wait(&sem);
    for (char i = 'a'; i <= 'z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000);
    }
    cout << endl;
    sem_post(&sem);
    return NULL;
}
// 两个线程一定交替进行
int main() {
    pthread_t tid1, tid2;
    int ret = -1;
    // 初始化一个信号量
    ret = sem_init(&sem, 0, 1);
    if (ret != 0) {
        cout << "sem_init failed..." << endl;
        exit(1);
    }
    cout << "init a semaphore ok..." << endl;
    // 创建两个线程
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL); // 最好加上判定函数是否成功执行
    pthread_join(tid2, NULL);

    cout << "main thread exit..." << endl;
    // 销毁sem
    sem_destroy(&sem);
    return 0;
}