#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("111111\n");
    int fd = open("1.txt", O_WRONLY | O_TRUNC);

    write(fd, "99999", 5);

    // dup函数：复制文件描述符
    int new_fd = dup(fd);
    // 会申请一个新的文件描述符，指向同一个对象

    write(fd, "3333", 4);
    write(new_fd, "4444", 4);
    
    printf("fd = %d\n", fd);
    printf("fd = %d\n", new_fd);
    
    close(fd);
    close(new_fd);

    return 0;
}

