#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("1.txt", O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");

    // 使用 mmap 将文件内容映射到内存
    // 参数含义：
    // 1. NULL       -> 让内核自动选择映射的起始地址
    // 2. 10         -> 映射 10 个字节
    // 3. PROT_READ  -> 这段映射内存是“可读”的
    // 4. MAP_SHARED -> 共享映射；若可写并修改，可能同步到底层文件
    // 5. fd         -> 要映射的文件描述符
    // 6. 0          -> 从文件偏移量 0 开始映射
    //
    // 返回值：
    //   成功：返回映射区的起始地址
    //   失败：返回 MAP_FAILED（通常是 (void *)-1）
    char *p = (char *)mmap(NULL, 10, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap failed");
    
    // 遍历打印映射区中的前十个字节
    for(int i=0; i< 10; i++){
        printf("%c-----\n", p[i]);
    }
    
    // 解除内存映射
    // 参数：
    // 1. p   -> 映射区起始地址
    // 2. 10  -> 映射区长度（必须和 mmap 映射长度一致）
    // 返回值：
    //   成功：0
    //   失败：-1    
    munmap(p, 10);
    // 关闭文件描述符，释放内核资源
    close(fd);
   
    return 0;
}

