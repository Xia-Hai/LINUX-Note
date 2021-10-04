#include <iostream>
#include <cstring>
#include <pthread.h>

using namespace std;

void *fun(void *arg)
{
    cout << "新的线程开始执行: " << pthread_self() << endl;
    return NULL;
}

void *fun2(void *arg) {
    int var = (int)(long)arg;
    cout << "线程2 var ：" << var << endl;
    return NULL;
}

int main()
{
    int ret = -1;
    pthread_t tid = -1;
    pthread_t tid2 = -1;
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (ret != 0)
    {
        cout << "pthread_creat failed..." << endl;
        exit(1);
    }

    ret = pthread_create(&tid2, NULL, fun2, (void*)0x3);
    if (ret != 0)
    {
        cout << "pthread_creat failed..." << endl;
        exit(1);
    }
    cout << "main thread..." << pthread_self() << endl;
    cout << "按下任意键退出主线程" << endl;
    getchar();
    return 0;
}