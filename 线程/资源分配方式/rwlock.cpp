#include <iostream>
#include <pthread.h>
#include <unistd.h> // sleep

using namespace std;
pthread_rwlock_t rwlock;
int num = 0; // 全局变量
// read thread
void *fun_read(void *arg) {
    // 获取线程编号
    int index = (int)(long)arg;
    while (1) {
        // 加读写锁
        pthread_rwlock_rdlock(&rwlock);
        cout << "thread " << index << " read num " << num << endl;
        // 随机睡眠 1- 3 秒
        pthread_rwlock_unlock(&rwlock);
        sleep(rand() % 3 + 1);
    }
    return NULL;
}
// write thread
void *fun_write(void *arg) {
    int index = (int)(long)arg;
    while (1) {
        // 加读写锁的写锁
        pthread_rwlock_wrlock(&rwlock);
        ++num;
        cout << "thread " << index << " modify num " << num << endl;
        pthread_rwlock_unlock(&rwlock);
        sleep(rand() % 3 + 1);
    }
    return NULL;
}

int main() {
    int ret = -1;
    ret = pthread_rwlock_init(&rwlock, NULL);
    if (ret != 0) {
        cout << "rwlock_init failed..." << endl;
        exit(1);
    }
    // 设置随机种子
    //srand(time(NULL)); 
    srand(getpid());
    // 创建 8 个线程
    pthread_t tid[8];
    for (int i = 0; i < 8; i++) {
        if (i < 5) {
            pthread_create(&tid[i], NULL, fun_read, (void *)(long)i);
        }
        else {
            pthread_create(&tid[i], NULL, fun_write, (void *)(long)i);
        }
    }
    // 回收 8 个线程的资源
    for (int i = 0; i < 8; i++) {
        pthread_join(tid[i], NULL);
    }
    // 销毁读写锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}