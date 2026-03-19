#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("6.txt", O_WRONLY | O_CREAT | O_TRUNC, 0654);
    ERROR_CHECK(fd, -1, "open failed");

    ssize_t ret = write(fd, "hello", 4);
    // 这里write函数中的n表示，这次实际写多少数据出去
    // read函数中的n表示最多能装n个数据

    printf("ret = %ld", ret);

    // 一定要注意read write的n的含义区别
    // read的n表明数组的长度，或者最多能装多少，实际能装多少，取决于文件还剩多少
    // write的n表明数据要写出多少
    
    close(fd);
    return 0;
}

