#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 之前使用fopen打开有缓冲区文件流
    // 现在使用open打开无缓冲区文件流
     
    printf("%o\n", O_RDONLY);
    printf("%o\n", O_WRONLY);
    printf("%o\n", O_RDWR);
    printf("%o\n", O_APPEND);    
    
    return 0;
}

