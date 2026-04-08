#include <my_header.h>
#include "queue.h"
#include "thread_pool.h"
#include "worker.h"

//初始化线程池中的数据
void init_thread_pool(thread_pool_t *pool, int num){                                  
    
    //初始化子线程的数目
    pool->thread_num = num;

    //初始化互斥锁
    pthread_mutex_init(&pool->lock, NULL);

    //初始化条件变量
    pthread_cond_init(&pool->cond, NULL);

    //初始化队列
    memset(&pool->queue, 0, sizeof(queue_t));

    //给线程id对应的指针分配空间
    pool->thread_id_arr = (pthread_t *)malloc(num * sizeof(pthread_t));

    for(int idx=0; idx<num; ++idx){
        //一定要注意,要把pool变量传入至thread_func中,因为会用到互斥锁、条件变量等
        int ret = pthread_create(&pool->thread_id_arr[idx], NULL, thread_func, (void *)pool);
        ERROR_CHECK(ret, -1, "pthread_create");
    }
}
