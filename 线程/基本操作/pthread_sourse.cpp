#include <iostream>
#include <pthread.h>
#include <cstring>

using namespace std;

// 验证数据段和堆
int num = 100;

void *fun(void *arg) {
    int *pn = (int *)arg;
    cout << "before fun num: " << num << endl;
    cout << "before fun *p: " << *pn << endl;
    ++num;
    ++(*pn);
    cout << "after fun num: " << num << endl;
    cout << "after fun *p: " << *pn << endl;

    return nullptr;
}

int main() {
    pthread_t tid;
    int ret = -1;
    memset(&tid, 0, sizeof(tid));
    int *p = new int(200); // 分配堆空间

    // 创建一个线程
    ret = pthread_create(&tid, NULL, fun, (void *)p);
    if (ret != 0) {
        cout << "pthread_creat failed..." << endl;
        exit(1);
    }
    cout << "按下任意键 函数继续" << endl;
    getchar();
    cout << "main num: " << num << endl;
    cout << "main *p: " << *p << endl;
    delete p; // 释放空间
    return 0;
}