#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 假设现在有一个 10GB 的大文件
    // 不能一次性 mmap（可能受限于地址空间）
    // 因此需要“分段映射 + 分段拷贝”
    // -----------------------------

    // 使用 mmap 实现大文件复制的大致流程：
    // 1. 获取文件大小（使用 fstat）
    // 2. 按块（例如 1GB 或更小）循环 mmap
    // 3. 每一段拷贝到目标文件
    // 4. 解除映射 munmap

    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open failed");
    // 定义 stat 结构体，用于存储文件信息
    struct stat stat_buf;
    // 使用fstat通过已经打开的文件描述符获取信息
    fstat(fd, &stat_buf);
    // 打印文件大小
    // 注意：st_size 类型通常是 off_t（可能是 long 或 long long）
    printf("%ld", stat_buf.st_size);
    
    return 0;
}

