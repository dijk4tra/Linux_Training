#include <my_header.h>

void *thread_func(void *arg){
    char *pp = (char *)arg;
    printf("I am son\n");
    strcpy(pp, "hello from child thread");
    return NULL;
}

int main(int argc, char *argv[]){                                  
    
    char *ptmp = (char *)malloc(50); //堆空间

    //主线程: main线程
    //子线程: 通过pthread_create创建的线程
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    int ret = pthread_create(&thread_id, NULL, thread_func, (void *)ptmp); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("I am main\n");
    sleep(5);
    
    printf("ptmp: %s\n", ptmp);
    
    free(ptmp);
    ptmp = NULL; //保持好习惯

    return 0;
}

