#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 在重定向这节，任何操作之前，先输出一次
    // 注意：一定要有 \n ！
    printf("Hello world\n");

    int fd = open("1.txt", O_RDONLY);
    printf("fd = %d\n", fd);
    
    return 0;
}

