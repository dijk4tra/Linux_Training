#include <my_header.h>
#include "thread_pool.h"
#include "queue.h"
#include "worker.h"

void init_thread_pool(thread_pool_t *pool, int num){
    
    pool->exitFlag = 0;

    pool->thread_num = num;

    memset(&pool->queue, 0, sizeof(queue_t));
    
    pthread_mutex_init(&pool->lock, NULL);

    pthread_cond_init(&pool->cond, NULL);

    pool->thread_id_arr = (pthread_t *)malloc(num * sizeof(pthread_t));
    
    //循环创建指定数量的子线程
    for(int idx=0; idx<num; ++idx){
        int ret = pthread_create(&pool->thread_id_arr[idx], NULL, thread_func, (void *)pool);
        ERROR_CHECK(ret, -1, "pthread_create");
    }

}
