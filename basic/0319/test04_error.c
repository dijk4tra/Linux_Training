#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");

    char *p = (char *)mmap(NULL, 10, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap failed");

    // 如果文件只有0字节。 现在要访问 前10个字节。 这时候会出Bus error .
    // 这个一般是在一些你访问到了不该访问的内存上会出现
    for(int i=0; i<100; i++){
        printf("%c-----\n", p[i]);
    }
    munmap(p, 10);
    close(fd);
    

    return 0;
}

