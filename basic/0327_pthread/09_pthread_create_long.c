#include <my_header.h>

void *thread_func(void *arg){
    printf("I am son\n");
    
    long tmp = (long)arg;
    tmp = 30;
    printf("tmp: %ld\n", tmp);
    
    return NULL;
}


int main(int argc, char *argv[]){                                  
    
    //特殊情况: long型
    long num = 10;
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    //这里传参本质上要传8个字节(一个指针,在64位系统,一个指针是8字节),那就直接传大小为8字节的long即可
    int ret = pthread_create(&thread_id, NULL, thread_func, (void *)num); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("I am main\n");
    printf("***num: %ld\n", num);

    sleep(5); //为了让主线程等待子线程执行完毕

    printf("num: %ld\n", num);
    
    return 0;
}

