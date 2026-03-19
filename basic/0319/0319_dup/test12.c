#include <my_header.h>

int main(int argc, char *argv[]){
    printf("---------\n");
    int fd = open("1.txt", O_WRONLY | O_TRUNC);
    ERROR_CHECK(fd, -1, "open failed");
    printf("11111\n");

    int stdout_backup_fd = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup(fd);
    printf("22222\n");

    close(STDOUT_FILENO);
    // 如果1这个文件描述符，没有对应的文件对象。 这时候输出会丢失
    printf("33333\n");

    close(fd);
    close(stdout_backup_fd);
    close(STDOUT_FILENO);
    
    return 0;
}


