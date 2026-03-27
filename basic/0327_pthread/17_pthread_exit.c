#include <my_header.h>

void func(){
    printf("func\n");
    pthread_exit(NULL);//会让线程提前执行结束，那么后面的代码不会执行
    printf("+++++\n");
}

void *thread_func(void *arg){
    printf("I am son\n");
    func();
    printf("func over int thread_func\n");
    return NULL;
}


int main(int argc, char *argv[]){                                  
    
    pthread_t thread_id;
    //pthread_create的第四个参数可以将值传递给线程入口参数
    int ret = pthread_create(&thread_id, NULL, thread_func, NULL); //创建了子线程                                                                       
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("I am main\n");
    
    ret = pthread_join(thread_id, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    printf("over\n");

    return 0;
}

