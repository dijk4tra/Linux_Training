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
    
    char content[] = "hello from useraaaaaa";
    write(write_fd, content, sizeof(content));

    char buf[100] = {0};
    read(read_fd, buf, sizeof(buf));
    printf("usera get msg: %s\n", buf);

    // read write 的len的语义是不一样的
    // read 的len是最多能装多少
    // write是实际要写出去多少
    
    close(write_fd);
    close(read_fd);

    
    return 0;
}

