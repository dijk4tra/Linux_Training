#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 孤儿进程、僵尸进程, 讲的是在fork的时候, 创建出来的进程的一些特殊情况
    // 孤儿进程: 父进程先结束, 子进程迟迟不结束, 这时候子进程就被称为孤儿进程
    // 孤儿进程, 会被一个进程(pid = 1)收养
    
    // 如果子进程先退出，系统不会自动清理掉子进程的环境，而必须由父进程调用wait或waitpid函数来完成清理工作
    // 僵尸进程: 如果父进程不做清理工作，则已经退出的子进程将成为僵尸进程(defunct)
    // 在系统中如果存在的僵尸进程过多，将会影响系统的性能， 所以必须对僵尸进程进行处理
    // 僵尸进程: 处于Z状态的子进程
    
    // 创建子进程
    pid_t ret = fork();

    if(ret == 0){
        printf("child process.\n");
        while(1){
            printf("child process runnig.\n");
            sleep(1);
        }

    }else{
        printf("father process.\n");

        sleep(5);
        printf("father process over.\n");
    }

    return 0;
}

