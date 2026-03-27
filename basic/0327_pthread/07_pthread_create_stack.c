#include <my_header.h>

void *thread_func(void *arg){
    printf("I am son\n");
    int *pint = (int *)arg;
    *pint = 30;
    return NULL;
}

int main(int argc, char *argv[]){                                  
    
    int num = 10; //主线程中的变量10

    //主线程: main线程
    //子线程: 通过pthread_create创建的线程
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    int ret = pthread_create(&thread_id, NULL, thread_func, (void *)&num); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("***num: %d\n", num);
    printf("I am main\n");
    sleep(5);
    
    printf("num: %d\n", num);
    
    return 0;
}

