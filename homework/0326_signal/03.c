#include <my_header.h>

/* 
 *使用sigaction注册2号信号的处理函数
 * 1)允许自动重启低速系统调用;
 * 2)在处理2号信号时阻塞3号信号,不会被3号信号打断;
 * 3)在2号信号的信号处理函数中,使用sigpending函数判断一下有没有3号信号处于未决状态;
 */

void func(int num){
    printf("\n收到信号 num: %d\n", num);
}

int main(int argc, char *argv[]){                                  
    
    struct sigaction act, oldact;
    
    memset(&act, 0, sizeof(act));
    
    act.sa_handler = func;
    act.sa_flags = SA_RESTART;

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, 3);

    return 0;
}

