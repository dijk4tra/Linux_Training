#include <my_header.h>

/*
 * 程序功能：
 * 1. 先打开文件 1.txt，准备把内容写进去。
 * 2. 备份当前标准输出 stdout（本来默认指向终端/屏幕）。
 * 3. 通过 close(1) + dup(fd) 的方式，让标准输出重定向到文件。
 * 4. 再通过 close(1) + dup(stdout_backup_fd) 恢复标准输出到屏幕。
 * 5. 重复几次这个过程，观察 printf 的输出到底去了哪里。
 *
 * 说明：
 * - STDOUT_FILENO 的值固定是 1，表示标准输出。
 * - dup(oldfd) 会复制一个文件描述符，并返回“当前最小可用”的新文件描述符。
 * - 所以当我们先 close(1) 后，1 就变成“最小可用描述符”，
 *   这时 dup(fd) 往往就会返回 1。
 * - 一旦返回 1，就相当于“标准输出被改绑到 fd 对应的文件上”。
 */

int main(int argc, char *argv[]){                                  
    
    printf("------------\n");
    int fd = open("1.txt", O_WRONLY | O_TRUNC);
    /*
     * 打开文件 1.txt：
     * - O_WRONLY：只写打开
     * - O_TRUNC ：如果文件存在且成功打开，则把文件长度截断为 0
     *
     * 注意：
     * 这里只用了 O_WRONLY | O_TRUNC，没有 O_CREAT，
     * 所以要求 1.txt 必须已经存在，否则 open 会失败。
     *
     * open 成功时返回一个新的文件描述符，通常是 3：
     * - 0 标准输入 stdin
     * - 1 标准输出 stdout
     * - 2 标准错误 stderr
     * - 3 往往是程序打开的第一个普通文件
     *
     */    
    // 这里fd为3 
    ERROR_CHECK(fd, -1, "open failed");
    
    printf("11111\n");
    // 此时标准输出仍然指向屏幕，所以 11111 会打印到终端
    
    int stdout_backup_fd = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    // 现在文件描述符1(标准输出)被释放，处于可用状态    
    dup(fd);
    // 现在文件描述符1和fd对应关系相同，即都指向1.txt
    printf("22222\n");

    close(STDOUT_FILENO);
    // 文件描述符1又被释放，处于可用状态
    dup(stdout_backup_fd);
    // 现在文件描述符1的指向重新变为屏幕
    printf("33333\n");
    
    close(STDOUT_FILENO);
    // 文件描述符1又被释放，处于可用状态
    dup(fd);
    // 现在文件描述符1的指向变为1.txt
    printf("44444\n");

    close(STDOUT_FILENO);
    // 文件描述符1又被释放，处于可用状态
    dup(stdout_backup_fd);
    // 现在文件描述符1的指向重新变为屏幕
    printf("55555\n");
    
    // 关闭文件描述符fd
    close(fd);

    return 0;
}

