#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("User B: Opening pipes...\n");
    // B 的读写顺序要和 A 对应
    int read_fd = open("1.pipe", O_RDONLY);
    int write_fd = open("2.pipe", O_WRONLY);

    error_check(read_fd, -1, "open 1.pipe failed");
    error_check(write_fd, -1, "open 2.pipe failed");
    
    printf("User B: Ready\n");

    while(1) {
        // 1. 先接收 User A 的消息
        char buf2[100] = {0};
        read(read_fd, buf2, sizeof(buf2));
        printf("Msg from User A: %s\n", buf2);

        // 2. 从键盘读取输入并回复
        printf("User B, please input msg: \n");
        char buf[100] = {0};
        ssize_t ret = read(STDIN_FILENO, buf, sizeof(buf));
        write(write_fd, buf, ret);
    }

    close(write_fd);
    close(read_fd);

    return 0;   
}

