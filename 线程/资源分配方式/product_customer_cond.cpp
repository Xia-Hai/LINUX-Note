#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
// 条件变量和互斥量
pthread_mutex_t mutex;
pthread_cond_t cond;
struct node_t {
    int data;
    node_t *next;
    node_t() : data(0), next(nullptr) {}
    node_t(int num) : data(num), next(nullptr) {}
};
// 创建一个头结点 head;
node_t *head = nullptr;

// 生产者消费者模型，条件变量实现
void *producer(void *arg) {
    // 循环生产产品
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        node_t *newnode = new node_t(rand() % 100 + 1);
        cout << "产生结点..." << newnode->data << endl;
        // 头插法
        newnode->next = head;
        head = newnode;
        pthread_mutex_unlock(&mutex);
        // 唤醒
        pthread_cond_signal(&cond);
        sleep(rand() % 3 + 1);
    }

    pthread_exit(NULL); // return NULL;
}

void *customer(void *arg) {
    // 循环消费
    node_t *tmp = nullptr;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (head == nullptr) {
            // 等待
            //cout << "链表为空 ..." << endl;
            pthread_cond_wait(&cond, &mutex);
        }
       
             // 删除第一个结点
        tmp = head;
        head = head->next;
        cout << "删除结点..." << tmp->data << endl;
        delete tmp;

        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1);
        
    }
   
    
    pthread_exit(NULL);
}
int main() {
    srand(getpid());
    //  初始化条件变量和互斥量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    // 创建两个线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, customer, NULL);



    // 等待线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 销毁互斥量和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}