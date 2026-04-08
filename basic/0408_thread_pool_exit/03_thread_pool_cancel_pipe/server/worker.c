#include "worker.h"
#include "queue.h"
#include "thread_pool.h"
#include "send_file.h"

void cleanup_lock(void *p){
    pthread_mutex_t *lock = (pthread_mutex_t *)p;
    pthread_mutex_unlock(lock);
}

void *thread_func(void *arg){                                  
    //通过pthread_create函数的第三个参数传递过来之后
    //在线程入口函数中将其反着解出来
    thread_pool_t *pool = (thread_pool_t *)arg;

    //线程需要执行的逻辑
    while(1){
        
        //要特别注意: pthread_cleanup清理函数是“带有大括号”的宏!
        //这两个宏在预编译展开后，会在代码中插入一对大括号
        //在这对大括号中声明的变量就会变成局部变量,不能大括号外使用!
        //所以需要把变量fd的声明放在这两个宏包裹的范围之外!
        //否则当代码走出这对大括号后,fd的生命周期就结束并被销毁了
        //这会导致后面执行send_file(fd)时出错,编译器会找不到这个变量
        int fd = 0;

        //从队列中拿数据, 因为队列是共享资源, 所以需要上锁
        pthread_mutex_lock(&pool->lock);
        pthread_cleanup_push(cleanup_lock, &pool->lock);
        
        //如果队列是空的, 工作线程只能睡眠
        //只有队列不空的时候, 才能从队列中取数据
        //面试常考: 什么是虚假唤醒? 如何解决虚假唤醒的问题?
        while(pool->queue.size == 0){
            pthread_cond_wait(&pool->cond, &pool->lock);
        }

        //只有队列不空的时候, 才能从队列中取数据
        //获取文件描述符
        fd = pool->queue.head->fd;
        //出队
        deQueue(&pool->queue);

        //后续要解锁
        /* pthread_mutex_unlock(&pool->lock); */
        pthread_cleanup_pop(1);

        //利用文件描述符将数据发给客户端
        send_file(fd);

        close(fd);
    }
    
    return 0;
}

