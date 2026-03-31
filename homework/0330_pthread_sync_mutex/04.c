#include <my_header.h>

/*
 * 现在有两个线程t1和t2，t1 打印 A 和 C，t2 打印 B。
 * 书写代码，使用条件变量每次的显示顺序都是A->B->C。
 * 代码书写完成后，回答问题：条件变量中加锁的目的是什么？
 */

// 逻辑：A (status 0) -> B (status 1) -> C (status 2)
// 状态标识(共享资源): 0打印A, 1打印B, 2打印C
int status = 0;

// 定义保护共享资源的互斥锁
pthread_mutex_t mutex;
// 定义用于线程间通信的条件变量
pthread_cond_t cond;

void *thread_t1(void *arg){
    // 打印A
    pthread_mutex_lock(&mutex); // 访问 status 前加锁
    while(status != 0){         // 只要不是轮到打印 A，就等待（防止虚假唤醒）
        pthread_cond_wait(&cond, &mutex); // 释放锁并进入睡眠，唤醒时自动重新拿锁
    }
    printf("A\n");
    status = 1; // 修改状态为 1，准备让thread_t2打印B
    pthread_cond_broadcast(&cond); // 唤醒所有在等待条件变量的线程(让thread_t2去打印B)
    pthread_mutex_unlock(&mutex);  // 释放锁
    
    // 打印C
    pthread_mutex_lock(&mutex);
    while(status != 2){ // 只要不是轮到 C，就等待
        pthread_cond_wait(&cond, &mutex);
    }
    printf("C\n");
    status = 0;         // 打印结束，重置状态
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

void *thread_t2(void *arg){
    // 打印B
    pthread_mutex_lock(&mutex);
    while(status != 1){ // 只要不是轮到打印 B，就等待
        pthread_cond_wait(&cond, &mutex);
    }
    printf("B\n");
    status = 2; // 修改状态为 2，准备让thread_t1打印C
    pthread_cond_broadcast(&cond); // 唤醒thread_t1去打印C
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[]){                                  
    pthread_t pid_t1, pid_t2;
    // 初始化互斥锁
    int ret = pthread_mutex_init(&mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_t");
    // 初始化条件变量
    ret = pthread_cond_init(&cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_cond_init");
    // 创建线程
    pthread_create(&pid_t1, NULL, thread_t1, NULL);
    pthread_create(&pid_t2, NULL, thread_t2, NULL);
    // 等待线程回收
    pthread_join(pid_t1, NULL);
    pthread_join(pid_t2, NULL);

    // 资源清理
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}

