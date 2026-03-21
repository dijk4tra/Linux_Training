#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("father process, pid = %d\n", getpid());

    pid_t ret = fork();

    if(ret == 0){
        // 子进程
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }else{
        // 父进程
        printf("father process, pid = %d, ppid = %d, ret = %d\n", getpid(), getppid(), ret);
    }
    
    sleep(30);

    return 0;
}

