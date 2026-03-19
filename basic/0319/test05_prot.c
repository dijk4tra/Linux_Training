#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open failed");
    
    // int prot: 映射的内存区域的访问权限标志
    // PROT_READ: 区域可被读取
    // PROT_WRITE: 区域可被写入
    // PROT_READ|PROT_WRITE: 映射区域可读可写
    char *p = (char *)mmap(NULL, 10, PROT_WRITE | PROT_READ , MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap failed");

    for(int i=0; i<10; i++){
        printf("%c-----\n", p[i]);
        p[i] = 'a' + i;
    }
    munmap(p, 10);
    close(fd);
    
    return 0;
}

