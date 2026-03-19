#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("1.txt", O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");

    /*
     * mmap函数作用：
     *   将文件内容映射到进程的虚拟内存空间中。
     *   映射之后，就可以像访问普通内存一样访问文件内容。
     *
     * 原型：
     *   void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
     *
     * 参数说明：
     *   addr   = NULL
     *            表示让内核自动选择映射到哪个虚拟地址
     *
     *   length = 10
     *            表示映射 10 个字节
     *
     *   prot   = PROT_READ
     *            表示这段映射内存可读
     *
     *   flags  = MAP_SHARED
     *            表示共享映射
     *            如果是可写映射并修改内容，修改会同步到底层文件
     *
     *   fd     = fd
     *            要映射的文件描述符
     *
     *   offset = 0
     *            从文件开头开始映射
     *
     * 返回值：
     *   成功：返回映射区起始地址
     *   失败：返回 MAP_FAILED
     *
     * 注意：
     *   offset 必须是系统页大小的整数倍，通常是 4096 的倍数。
     */    
    char *p = (char *)mmap(NULL, 10, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap failed");
    // offset的值，只能是 4096 的倍数

    for(int i=0; i< 10; i++){
        printf("%d-----\n", p[i]);
    }

    munmap(p, 10);
    close(fd);

    return 0;
}

