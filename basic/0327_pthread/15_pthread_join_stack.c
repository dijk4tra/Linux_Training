#include <my_header.h>

void *thread_func(void *arg){
    printf("I am son\n");
    
    int num = 100;

    return (void *)&num; //局部变量的地址
                         //函数结束之后局部变量已经被销毁
}


int main(int argc, char *argv[]){                                  
    
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    int ret = pthread_create(&thread_id, NULL, thread_func, NULL); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("I am main\n");
    
    void *retval;
    ret = pthread_join(thread_id, &retval);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    printf("over\n");

    int *ptmp = (int *)retval;
    printf("ptmp: %d\n", *ptmp);
    
    return 0;
}

