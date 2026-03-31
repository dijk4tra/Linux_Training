#include <my_header.h>

/*
主线程执行事件A
A:
   printf("Before A!\n");
   sleep(3)
   printf("After A\n");

子线程执行事件B，
B:
   printf("Before B!\n");
   sleep(3)
   printf("After B\n");

在AB事件之外不使用sleep，使用循环机制
配合互斥锁，确保B一定在A完成之后运行。
*/

// 共享资源:状态标识
int status = 0;

// 定义保护共享资源的互斥锁
pthread_mutex_t mutex;
// 定义用于线程间通信的条件变量
pthread_cond_t cond;

void *thread_func(void *arg){
    pthread_mutex_lock(&mutex);
    while(status != 1){
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Before B!\n");
    sleep(3);
    printf("After B\n");
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main(int argc, char *argv[]){                                  
    // 定义线程ID
    pthread_t tid;
    // 初始化互斥锁
    int ret = pthread_mutex_init(&mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    // 初始化条件变量
    ret = pthread_cond_init(&cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_cond_init");
    
    // 创建子线程
    ret = pthread_create(&tid, NULL, thread_func, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("Before A!\n");
    sleep(3);
    printf("After A\n");
    
    pthread_mutex_lock(&mutex);
    status = 1;
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);
    
    pthread_join(tid, NULL); // 主线程等待子线程执行结束
    
    // 资源回收
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}

