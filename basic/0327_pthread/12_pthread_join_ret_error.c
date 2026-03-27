#include <my_header.h>

void *thread_func(void *arg){
    printf("I am son\n");
    
    int *pInt = (int *)malloc(4);
    *pInt = 10;

    return (void *)pInt;
}


int main(int argc, char *argv[]){                                  
    
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    int ret = pthread_create(&thread_id, NULL, thread_func, NULL); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("I am main\n");
    
    /* void **retval =(void **)malloc(100); //正确写法 */
    void **retval; //错误写法
    ret = pthread_join(thread_id, retval);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    printf("over\n");

    int ptmp = *(int *)*retval;
    printf("ptmp: %d\n", ptmp);
    
    return 0;
}

