#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 一定要保证两个管道是存在的：1.pipe 2.pipe
    
    printf("111111\n");
    int write_fd = open("1.pipe", O_WRONLY);
    printf("222222\n");
    int read_fd = open("2.pipe", O_RDONLY);
    printf("333333\n");

    ERROR_CHECK(write_fd, -1, "open 1.pipe failed");
    ERROR_CHECK(read_fd, -1, "open 2.pipe failed");
    
    while(1){
        // 先让a给b发，a从键盘捞信息回来
        printf("please input msg: \n");
        char buf[100] = {0};
        ssize_t ret = read(STDIN_FILENO, buf, sizeof(buf));

        // 再把信息发到管道中去
        printf("stdin input count = %ld\n", ret);
        write(write_fd, buf, ret);

        // 从管道中读消息
        char buf2[100] = {0};
        read(read_fd, buf2, sizeof(buf2));
        printf("msg from userb: %s\n", buf2);
        
    }
    
    close(write_fd);
    close(read_fd);
    
    return 0;
}

