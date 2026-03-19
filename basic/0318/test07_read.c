#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");
    
    char buf[10] = {0};
    ssize_t ret = read(fd, buf, sizeof(buf));
    printf("ret = %ld\n", ret);
    printf("%s-------\n", buf);

    // read 不会填充任何额外的数据，你里面有什么，就装入什么数据
    // 不会和其他函数一样末尾填入 \0
    // ret 是这次读取到的数据个数

    close(fd);

    return 0;
}

