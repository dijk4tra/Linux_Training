#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    FILE *fp = fopen("222.txt", "w");
    ERROR_CHECK(fp, NULL, "fopen failed");

    // fputs：向文件写入字符串（不自动加换行）
    fputs("hello", fp);
    // 向文件写入 "hello\n"
    // 注意：这是“库函数”（stdio），带缓冲区
    
    write(3, "world", 5);
    // write 是系统调用（unistd.h）
    // 原型：
    // ssize_t write(int fd, const void *buf, size_t count);
    //
    // 参数说明：
    // fd    ：文件描述符（file descriptor）
    // buf   ：要写入的数据地址
    // count ：要写入的字节数
    //
    // 重点：
    // 这里的 3 是“文件描述符”，不是 FILE*
    //
    // 在 Linux 中：
    // 0 -> stdin  （标准输入）
    // 1 -> stdout （标准输出）
    // 2 -> stderr （标准错误）
    //
    // fopen("222.txt","w") 底层会调用 open，
    // 通常会分配一个新的文件描述符（此时为 3）
    //
    // 所以：
    // write(3, "world", 5);
    // 实际上是往 "222.txt" 这个文件写入 "world"

    fclose(fp);
    // 关闭文件（同时刷新缓冲区，把数据真正写入磁盘）
    return 0;
}

