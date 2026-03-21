#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // select 简单来说，就是系统提供给你用于监听多个文件描述符的函数
    // read(STDIN_FILENO, ...)_监听(该文件描述符是否可用) + 捞数据
    // 文件描述符是否可用指的是：
    // STDIN_FILENO: 键盘中是否有数据
    // read_fd: 管道中是否有数据

    // select(10, NULL, NULL, NULL, NULL);

    fd_set readset;
    // 系统不希望大家了解其中的字段，但也想让大家使用
    // 所以系统提供了几个宏给大家使用
    FD_ZERO(&readset);
    // 将1这个文件描述符加入了 fd_set
    FD_SET(STDIN_FILENO, &readset);
    
    // select会阻塞，直到监听的文件描述符中至少一个可用时，才结束阻塞
    int ret = select(10, &readset, NULL, NULL, NULL);
    printf("ret = %d", ret);

    // 监听了多个文件描述符，哪一个文件描述符可用？
    // 入参是 *，没有用const修饰，信息是通过指针的方式带出，传入传出参数
    // 所以select结束之后，fd_set中的文件描述符，就是可用的文件描述符

    if(FD_ISSET(STDIN_FILENO, &readset)){
        // 说明文件描述符中fd_set中
        // STDIN_FILENO 文件描述符可用，说明键盘有信息
        
        char buf[100] = {0};
        read(STDIN_FILENO, buf, sizeof(buf));
        printf("read from stdin: %s\n", buf);

    }

    return 0;
}

