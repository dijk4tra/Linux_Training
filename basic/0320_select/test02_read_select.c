#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 这次监听两个文件描述符：STDIN_FILENO read_fd
    int read_fd = open("1.pipe", O_RDONLY);
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    FD_SET(read_fd, &set);

    int ret = select(read_fd + 1, &set, NULL, NULL, NULL);
    printf("select over, ret = %d\n", ret);

    if(FD_ISSET(STDIN_FILENO, &set)){
        printf("stdin enable.\n");
        char buf[100] = {0};
        read(STDIN_FILENO, buf, sizeof(buf));
        printf("STDIN input: %s\n", buf);
    }
    
    if(FD_ISSET(read_fd, &set)){
        printf("pipe enable.\n");
        char buf[100] = {0};
        read(read_fd, buf, sizeof(buf));
        printf("pipe input: %s\n", buf);
    }


    return 0;
}

