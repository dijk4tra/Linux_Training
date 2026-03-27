#include <my_header.h>

void *thread_func(void *arg){
    printf("***son thread id: %ld\n", pthread_self());
    printf("I am son\n");
    return NULL;
}

int main(int argc, char *argv[]){                                  
    
    //主线程: main线程
    //子线程: 通过pthread_create创建的线程
    pthread_t thread_id;
    int ret = pthread_create(&thread_id, NULL, thread_func, NULL); //创建了子线程
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("son thread id: %ld\n", thread_id);
    printf("main thread id: %ld\n", pthread_self());

    printf("I am main\n");
    while(1); //主线程会等待子线程
    return 0;
}

