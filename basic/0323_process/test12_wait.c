#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    if(fork() == 0){
        // 子进程
        printf("child process begin.\n");
        sleep(30);
        printf("child process over.\n");
        return 0;
    }else{
        printf("father process begin.\n");
        int wstatus;
        // wait的入参, 相当于就是获取子进程的状态, 看子进程是否正常退出
        // 正常退出, 退出码是多少; 不正常退出, 信号标志是多少, 都在wstatus中存
        pid_t wait_ret = wait(&wstatus);
        printf("father process over.\n");

        if(WIFEXITED(wstatus)){ // 子进程正常退出
            printf("child status: %d \n", WEXITSTATUS(wstatus));
        }else if(WIFSIGNALED(wstatus)){ // 子进程信号退出
            printf("child signed: %d \n", WTERMSIG(wstatus));
        }
    }

    return 0;
}

