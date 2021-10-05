#include <iostream>
#include <pthread.h>
#include <unistd.h> // usleep

using namespace std;
// 模拟输出字符

pthread_mutex_t mutex;

void *fun1(void *arg) {
    // 输出大写字母
    pthread_mutex_lock(&mutex);
    for (char i = 'A'; i <= 'Z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000); // 100ms
    }
    cout << endl;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *fun2(void *arg) {
    // 输出小写字母
    pthread_mutex_lock(&mutex);
    for (char i = 'a'; i <= 'z'; ++i) {
        cout << i << ' ' << flush;
        //fflush(stdout);
        usleep(100000);
    }
    cout << endl;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
// 两个线程一定交替进行
int main() {
    pthread_t tid1, tid2;
    int ret = -1;
    // 初始化一个互斥锁
    ret = pthread_mutex_init(&mutex, NULL);
    if (ret != 0) {
        cout << "pthread_mutex_init failed...";
        exit(1);
    }
    cout << "mutex inti success..." << endl;
    // 创建两个线程
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL); // 最好加上判定函数是否成功执行
    pthread_join(tid2, NULL);

    cout << "main thread exit..." << endl;
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);
    return 0;
}