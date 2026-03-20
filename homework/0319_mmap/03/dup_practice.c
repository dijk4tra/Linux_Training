#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("------------\n");
    int fd = open("1.txt", O_WRONLY | O_TRUNC );
    ERROR_CHECK(fd, -1, "open failed");

    int stdout_backup_fd = dup(STDOUT_FILENO);
    
    // 向屏幕输出数据
    printf("11111\n");
    
    // 释放文件描述符1
    close(STDOUT_FILENO);
    // 使文件描述符1的指向和fp相同(都指向文件1.txt)
    dup(fd);
    // 向文件1.txt输出数据
    printf("22222\n");
    
    // 释放文件描述符1
    close(STDOUT_FILENO);
    // 使文件描述符1的指向重新变为屏幕
    dup(stdout_backup_fd);
    // 向屏幕输出数据
    printf("33333\n");


    // 释放文件描述符1
    close(STDOUT_FILENO);
    // 使文件描述符1的指向和fp相同(都指向文件1.txt)
    dup(fd);
    // 向文件1.txt输出数据
    printf("44444\n");

    // 释放文件描述符1
    close(STDOUT_FILENO);
    // 使文件描述符1的指向重新变为屏幕
    dup(stdout_backup_fd);
    // 向屏幕输出数据
    printf("55555\n");

    // 关闭文件描述符fd
    close(fd);

    return 0;
}

