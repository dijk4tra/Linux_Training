#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 1.pipe 是文件
    // 打开写端
    printf("open before.\n");
    int fd = open("1.pipe", O_WRONLY);
    printf("open after.\n"); 

    return 0;
}

