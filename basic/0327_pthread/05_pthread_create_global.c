#include <my_header.h>

int gCnt = 0;

void *thread_func(void *arg){
    for(int idx=0; idx<1000000; ++idx){
        ++gCnt;
    }
    printf("I am son\n");
    return NULL;
}

int main(int argc, char *argv[]){                                  
    
    //主线程: main线程
    //子线程: 通过pthread_create创建的线程
    pthread_t thread_id;
    int ret = pthread_create(&thread_id, NULL, thread_func, NULL); //创建了子线程
    THREAD_ERROR_CHECK(ret, "pthread_create");

    for(int idx=0; idx<1000000; ++idx){
        ++gCnt;
    }

    printf("son thread id: %ld\n", thread_id);
    printf("main thread id: %ld\n", pthread_self());

    printf("I am main\n");
    sleep(5);
    
    printf("gCnt: %d", gCnt);
    return 0;
}

