#include <iostream>
#include <pthread.h>

using namespace std;

// pthread_self()

int main() {
    pthread_t tid = 0;

    // 获取当前线程的 id
    tid = pthread_self();

    cout << "tid : " << tid << endl;
    if (pthread_equal(tid, pthread_self()))
    {
        cout << "两个线程ID相同" << endl;
    }
    else 
    {
        cout << "两个线程ID不相同" << endl;
    }
    return 0;
}