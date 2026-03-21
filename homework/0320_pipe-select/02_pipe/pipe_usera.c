#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("User A: Opening pipes...\n");
    // 注意：open 管道时会阻塞，直到另一个进程也打开它
    int write_fd = open("1.pipe", O_WRONLY); 
    int read_fd = open("2.pipe", O_RDONLY);

    error_check(write_fd, -1, "open 1.pipe failed");
    error_check(read_fd, -1, "open 2.pipe failed");
    
    printf("User A: Ready\n");

    while(1) {
        // 1. 从键盘读取输入
        printf("User A, please input msg: \n");
        char buf[100] = {0};
        ssize_t ret = read(STDIN_FILENO, buf, sizeof(buf));

        // 2. 发送给 User B
        write(write_fd, buf, ret);

        // 3. 等待并接收 User B 的消息
        char buf2[100] = {0};
        read(read_fd, buf2, sizeof(buf2));
        printf("Msg from User B: %s\n", buf2);
    }

    close(write_fd);
    close(read_fd);

    return 0;
}

