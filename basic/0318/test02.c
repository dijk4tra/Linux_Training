#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 无缓冲区文件流：
    // 指的是，在用户空间没有了缓冲区，数据都是直接从内核中读取
    // 这时候，只提供了一个按照数组读取
    // 返回值问题：之前返回FILE *，现在不想返回FILE *
    // 返回了一个int，文件描述符
    // 系统内核是知道对应关系的

    printf("Hello world\n");
    
    return 0;
}

