#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // waitpid用于等待指定的子进程,不是所有的子进程都行,只等一个
    
    pid_t fork_ret = fork();

    if(fork_ret == 0){
        // 子进程
        printf("child process begin.\n");
        sleep(20);
        printf("child process over.\n");
    }else{
        // 父进程
        printf("father process begin.\n");
        
        // waitpid可以实现非阻塞等
        pid_t waitpid_ret = waitpid(fork_ret, NULL, 0);

        printf("wait over, ret = %d\n", waitpid_ret);
        printf("father process over.\n");
    }

    return 0;
}

