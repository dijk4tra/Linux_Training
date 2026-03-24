#include <my_header.h>

/*
 * 命名管道（FIFO）是什么：
 *     FIFO 是一种特殊文件，常用于进程间通信（IPC）。
 *     它和普通文件不同：
 *       1. 普通文件用于长期保存数据到磁盘；
 *       2. FIFO 主要用于“一个进程写、另一个进程读”的通信；
 *       3. 数据是先进先出（First In First Out）方式传输。
 *
 * mkfifo() 函数说明：
 *     原型：
 *         int mkfifo(const char *pathname, mode_t mode);
 *
 *     参数：
 *         pathname
 *             要创建的 FIFO 文件路径，这里是 "2.pipe"。
 *
 *         mode
 *             指定新建 FIFO 的权限，这里写的是 0666。
 *             0666 表示：
 *                 - 文件所有者可读可写
 *                 - 同组用户可读可写
 *                 - 其他用户可读可写
 *
 *             注意：
 *                 实际最终权限还会受到当前进程 umask 的影响。
 *                 比如 mode=0666，但如果 umask 是 0022，
 *                 最终权限可能会变成 0644。
 *
 *     返回值：
 *         成功：返回 0
 *         失败：返回 -1，并设置 errno
 *
 *     常见失败原因：
 *         1. 同名文件已经存在（errno 可能为 EEXIST）
 *         2. 没有目录写权限
 *         3. 路径非法或父目录不存在
 *
 * 使用场景：
 *     创建好 FIFO 之后，其他进程可以通过 open("2.pipe", O_RDONLY)
 *     或 open("2.pipe", O_WRONLY) 来读写它。
 *
 * 示例：
 *     程序运行后，当前目录下会出现一个特殊文件 2.pipe，
 *     可通过 ls -l 查看，开头通常会显示为 p，表示 pipe：
 *         prw-r--r-- 1 user user 0 ... 2.pipe
 */

int main(int argc, char *argv[]){                                  
    
    int ret = mkfifo("2.pipe", 0666);
    printf("ret = %d\n", ret);
    ERROR_CHECK(ret, -1, "mkfifo failed");

    return 0;
}

