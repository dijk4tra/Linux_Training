#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");

    char buf[10] = {0};
    ssize_t ret = read(fd, buf, sizeof(buf)-1);
    printf("ret = %ld\n", ret);
    printf("%s-------\n", buf);

    ssize_t ret2 = read(fd, buf, sizeof(buf)-1);
    printf("ret2 = %ld\n", ret);
    printf("%s-------\n", buf);
    
    // echo "123456789abc">5.txt
    // 这里实际上向文件中存入了13个数据 123456789abc\n
    // ./test08_read.c 5.txt


    // read 不会填充任何额外的数据，你里面有什么，就装入什么数据
    // 不会和其他函数一样末尾填入 \0
    // ret 是这次读取到的数据个数
    
    close(fd);

    return 0;
}

