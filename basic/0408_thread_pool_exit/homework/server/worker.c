#include <my_header.h>
#include "worker.h"
#include "queue.h"
#include "thread_pool.h"
#include "send_file.h"

void *thread_func(void *arg){
    // 通过 pthread_create 的第四个参数将 thread_pool_t 指针传过来，
    // 在这里进行强制类型转换解包，获得线程池的操作句柄。
    thread_pool_t *pool = (thread_pool_t *)arg;

    // 工作线程进入无限循环，不断地去队列取任务、执行任务
    while(1){
        int fd = 0;
        // 1. 尝试从队列拿数据前，必须先获取互斥锁（保护共享的队列结构）
        pthread_mutex_lock(&pool->lock);

        // 2. 检查队列是否为空
        while(pool->queue.size == 0 && pool->exitFlag == 0){
            // pthread_cond_wait 做了三件事：
            // 1. 自动解开互斥锁 (让主线程能加锁入队)
            // 2. 让当前线程挂起睡眠
            // 3. 当收到 signal / broadcast 唤醒信号并抢到锁时，重新对互斥锁加锁并往下执行
            pthread_cond_wait(&pool->cond, &pool->lock);
        }

        // 3. 检查是否收到了退出指令
        if(pool->exitFlag == 1){
            pthread_mutex_unlock(&pool->lock);
            pthread_exit((void *)NULL); // 结束本线程
        }

        // 4. 走到这里说明：队列不为空 (size > 0)，且不是要退出
        // 从队列头部取出客户端的网络文件描述符
        fd = pool->queue.head->fd;

        // 执行出队操作，将结点从队列中抹去
        deQueue(&pool->queue);
        
        // 5. 任务提取完毕，立即释放锁，让其他线程可以去队列取任务
        pthread_mutex_unlock(&pool->lock);

        send_file(fd);

        close(fd);
        // 执行完后，while(1) 循环又回到顶端，继续抢锁取下一个任务
    }

    return NULL;
}
