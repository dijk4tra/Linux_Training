#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 1. 打开管道（确保 mkfifo 已经创建了 1.pipe 和 2.pipe）
    int write_fd = open("1.pipe", O_WRONLY);
    int read_fd = open("2.pipe", O_RDONLY);

    if (write_fd == -1 || read_fd == -1) {
        perror("open pipe failed");
        return -1;
    }

    printf("User A is ready. You can type at any time!\n");

    fd_set rdset; // 定义一个文件描述符集合
    char buf[1024];

    while (1) {
        // 2. 清空集合并将需要监控的描述符加入
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset); // 监控键盘
        FD_SET(read_fd, &rdset);      // 监控管道（对方发来的消息）

        // 3. select 阻塞等待，直到有数据可读
        // 参数1是集合中最大描述符+1
        int ret = select(read_fd + 1, &rdset, NULL, NULL, NULL);

        if (ret > 0) {
            // 情况A：如果是键盘有输入
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                memset(buf, 0, sizeof(buf));
                int n = read(STDIN_FILENO, buf, sizeof(buf));
                write(write_fd, buf, n); // 发送给对方
            }

            // 情况B：如果是管道有数据读入（对方发来消息）
            if (FD_ISSET(read_fd, &rdset)) {
                memset(buf, 0, sizeof(buf));
                int n = read(read_fd, buf, sizeof(buf));
                if (n == 0) { // 对方关闭了管道
                    printf("User B left the chat.\n");
                    break;
                }
                printf("From User B: %s", buf);
            }
        }
    }

    close(write_fd);
    close(read_fd);
    return 0;

    return 0;
}

