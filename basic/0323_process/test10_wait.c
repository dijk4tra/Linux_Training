#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
   
    // wait用于父进程等待子进程, 一定要有子进程再wait
    /* int ret = (wait(NULL)); */
    /* printf("ret = %d\n", ret); */
    
    // 所以使用wait, 一定要保证有子进程

    pid_t ret = fork();
    if(ret == 0){
        // 说明这是子进程
        printf("child process.\n");
        sleep(10);
        printf("child process over.\n");
    }else{
        printf("father process.\n");
        sleep(2);
        // 父进程一定要等来子进程才能继续往下走
        // 需要用wait
        printf("father process sleep over.\n");

        // wait用于等待子进程, 但是等谁不知道, 谁先执行好, 就等谁
        wait(NULL);
        printf("father process over.\n");
    }

    return 0;
}

