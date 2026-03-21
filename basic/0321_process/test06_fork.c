#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // fork的返回值问题
    pid_t ret = fork();

    // 这个位置有几个进程执行
    // 靠返回值ret来确定当前是在父进程中, 还是在子进程中
    // ret是0, 表明当前在子进程中
    
    printf("ret = %d\n", ret);

    return 0;
}

