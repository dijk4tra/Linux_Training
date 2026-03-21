#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("fork before.\n");
    pid_t ret = fork();
    // fork 用于创建子进程。fork以下的代码，父进程、子进程都要执行
    printf("fork after.\n");

    return 0;
}

