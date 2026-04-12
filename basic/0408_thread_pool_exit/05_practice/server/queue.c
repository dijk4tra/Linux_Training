#include <my_header.h>
#include "queue.h"

int enQueue(queue_t *pQueue, int fd){
    node_t *pNew = (node_t *)calloc(1, sizeof(node_t));
    pNew->fd = fd;

    if(pQueue->size == 0){
        pQueue->head = pNew;
        pQueue->end = pNew;
    }else{
        pQueue->end->pNext = pNew;
        pQueue->end = pNew;
    }
    pQueue->size++;
    
    return 0;
}

int deQueue(queue_t *pQueue){
    
    if(pQueue->size == 0){
        printf("队列为空,无法出队");
        return -1;
    }

    //记录要删除的头结点
    node_t *p = pQueue->head;
    //头指针向后移位
    pQueue->head = p->pNext;

    //如果队列中本来就只有一个结点
    //删除之后队列为空,需要将尾结点置为空
    if(pQueue->size == 1){
        pQueue->end = NULL;
    }

    pQueue->size--;
    free(p);

    return 0;
}
