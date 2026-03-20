#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open failed");

    // 获取文件大小
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t file_length = stat_buf.st_size;

    // 使用mmap建立文件映射
    char *p = (char *)mmap(NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap failed");

    // 直接像操作数组一样操作文件内容
    for(size_t i = 0; i < file_length; i++){
        if(isupper(p[i])){
            p[i] = tolower(p[i]);
        }
    }
    
    // 释放文件映射
    munmap(p, file_length);
    // 释放文件描述符
    close(fd);

    return 0;
}

