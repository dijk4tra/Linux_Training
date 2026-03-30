#include <my_header.h>
/*
 *书写程序，创建3个子线程，然后等待所有的子线程终止，最后还要获取其退出状态
 */

void *child_thread(void *arg){
    int id = *(int *)arg;
    printf("子进程%d开始运行...\n", id);

    sleep(1);

    // 动态分配一个变量作为退出状态返回
    int *status = (int *)malloc(sizeof(int));
    *status = 11;
    printf("子线程%d即将结束,返回状态值: %d\n", id, *status);

    // 退出线程并返回状态指针
    pthread_exit((void *)status);
}

int main(int argc, char *argv[]){                                  
    
    pthread_t tids[3];  // 存储3个线程的 ID
    int thread_args[3]; // 存储传给线程的参数
    void *retval;       // 接收线程返回值的指针
    
    // 创建3个子线程
    for(int i=0; i<3; i++){
        thread_args[i] = i + 1;
        if(pthread_create(&tids[i], NULL, child_thread, &thread_args[i]) != 0){
            perror("pthread create failed.\n");
            return 1;
        }
    }
    
    printf("主线程正在等待所有子线程完成...\n");

    // 等待所有子线程终止并获取退出状态
    for (int i=0; i<3; i++){
        // pthread_join 会阻塞主线程，直到对应的 tids[i] 结束
        pthread_join(tids[i], &retval);
        int exit_status = *(int *)retval;
        printf("主线程回收了子线程 %d，获取到的退出状态为: %d\n", i + 1, exit_status);
        // 释放子线程中申请的内存，防止内存泄漏
        free(retval);
    }
    
    printf("\n所有子线程已退出，程序结束。\n");
    
    return 0;
}

