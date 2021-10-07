#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;
// 信号量变量
sem_t sem_producer;
sem_t sem_customer;
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
        // P操作
        sem_wait(&sem_producer);
        node_t *newnode = new node_t(rand() % 100 + 1);
        cout << "产生结点..." << newnode->data << endl;
        // 头插法
        newnode->next = head;
        head = newnode;
        // 消费者 V操作
        sem_post(&sem_customer);
       
        sleep(rand() % 3 + 1);
    }

    pthread_exit(NULL); // return NULL;
}

void *customer(void *arg) {
    // 循环消费
    node_t *tmp = nullptr;
    while (1) {
        // P 操作
        sem_wait(&sem_customer);
        if (head == nullptr) {
            // 等待
            cout << "链表为空 ..." << endl;
           
        }
        // 删除第一个结点
        tmp = head;
        head = head->next;
        cout << "删除结点..." << tmp->data << endl;
        delete tmp;

        sem_post(&sem_producer);

        sleep(rand() % 3 + 1);
        
    }
   
    
    pthread_exit(NULL);
}
int main() {
    srand(getpid());
    //  初始化信号量
    sem_init(&sem_producer, 0, 4);
    sem_init(&sem_customer, 0, 0);
   
    // 创建两个线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, customer, NULL);



    // 等待线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 销毁信号量
    sem_destroy(&sem_producer);
    sem_destroy(&sem_customer);
   

    return 0;
}