#include <iostream>
#include <pthread.h>
using namespace std;

// 演示死锁

// mutex argument
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

// thread 1
void *fun1(void *arg) {
    //  thread 1   s1 -> s2
    pthread_mutex_lock(&mutex1);
    cout << "thread 1 lock s1" << endl;
    pthread_mutex_lock(&mutex2);
    cout << "thread 1 lock s2" << endl;
    // thread 1 working
    cout << "thread 1 running..." << endl;

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}
// thread 2
void *fun2(void *arg) {
    //  thread 2   s2 -> s1
    pthread_mutex_lock(&mutex2);
    cout << "thread 2 lock s2" << endl;
    pthread_mutex_lock(&mutex1);
    cout << "thread 2 lock s1" << endl;
    // thread 2 working
    cout << "thread 2 running..." << endl;

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}


int main() {
    int ret = -1;
    pthread_t tid1, tid2;
    // init mutex
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);


    // 创建两个线程
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);
    
    // 回收资源
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
    // mutex destroy
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}