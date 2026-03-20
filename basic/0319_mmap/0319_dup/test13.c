#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("111111111111\n");

    int ret = dup2(STDOUT_FILENO, 10);
    
    // 返回值就是系统告诉你，dup2成功还是失败
    printf("fd = %d/n", ret);
    
    // 文件描述符有文件对象在使用，或者没有被使用
    // 1.如果该文件描述符没有被使用，直接给你
    // 2.如果该文件描述符已经被使用，则直接将该文件描述符原有的关系断掉，再返回文件描述符
    
    return 0;
}

