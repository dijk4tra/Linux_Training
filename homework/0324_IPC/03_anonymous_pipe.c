#include <my_header.h>

/*
 *   父子进程之间通过“两条匿名管道”实现双向通信：
 *   - 子进程 -> 父进程：一条管道
 *   - 父进程 -> 子进程：另一条管道
 *
 * 为什么需要两条管道？
 *   因为匿名管道本质上是“半双工”的，
 *   一条管道更适合只负责一个方向的数据流动。
 *
 * 如果希望：
 *      A 发给 B
 *      B 也能回给 A
 *   那么通常就需要两条管道：
 *      1. child_to_father_arr  : 子进程写，父进程读
 *      2. father_to_child_arr  : 父进程写，子进程读
 *
 * 匿名管道适用场景：
 *   这里依旧是父子进程通信，所以继续使用匿名管道最方便。
 *   因为 fork() 后，父子进程会继承这两条管道对应的文件描述符。
 *
 * 匿名管道 vs 命名管道（FIFO）：
 *   - 匿名管道：
 *       pipe() 创建，无路径名，通常用于有亲缘关系的进程
 *   - 命名管道：
 *       mkfifo() 创建，在文件系统中有名字，适合无亲缘关系进程通信
 *
 * 为什么不用命名管道？
 *   因为这里父子进程之间已经能通过 fork() 共享文件描述符，
 *   没必要再额外创建一个 FIFO 文件。
 *   匿名管道更轻量、代码更简洁。
 */

int main(int argc, char *argv[]){                                  
    
    /*
     * child_to_father_arr:
     *   用于 子进程 -> 父进程
     *   [0] 读端（父进程用）
     *   [1] 写端（子进程用）
     */
    int child_to_father_arr[2] = {0};
    
    /*
     * father_to_child_arr:
     *   用于 父进程 -> 子进程
     *   [0] 读端（子进程用）
     *   [1] 写端（父进程用）
     */
    int father_to_child_arr[2] = {0};

    pipe(child_to_father_arr);
    pipe(father_to_child_arr);

    pid_t ret = fork();

    if (ret == 0){
        /*
         * 子进程
         *
         * 子进程需要做两件事：
         *   1. 给父进程发消息  -> 使用 child_to_father_arr[1]
         *   2. 接收父进程消息 -> 使用 father_to_child_arr[0]
         *
         * 所以子进程中“用不到”的端应该关闭：
         *   father_to_child_arr[1]  // 这是父进程写给子进程的写端，子进程不用写
         *   child_to_father_arr[0]  // 这是子进程发给父进程的读端，子进程不用读
         *
         * 关闭无用端非常重要：
         *   1. 避免浪费文件描述符
         *   2. 明确每条管道的角色
         *   3. 避免某些场景下 read() 一直等不到结束条件
         */
        close(father_to_child_arr[1]);
        close(child_to_father_arr[0]);

        int read_fd = father_to_child_arr[0];
        int write_fd = child_to_father_arr[1];

        char msg[] = "hello frommmmmmmmmmm child";
        write(write_fd, msg, sizeof(msg));

        char buf[100] = {0};
        read(read_fd, buf, sizeof(buf));

        printf("child process read from pipe: %s\n", buf);

        close(write_fd);
        close(read_fd);
    }else{
        /*
         * 父进程
         *
         * 父进程需要做两件事：
         *   1. 接收子进程消息 -> 使用 child_to_father_arr[0]
         *   2. 给子进程发消息 -> 使用 father_to_child_arr[1]
         *
         * 所以父进程中“用不到”的端应该关闭：
         *   child_to_father_arr[1]  // 这是子进程写给父进程的写端，父进程不用写
         *   father_to_child_arr[0]  // 这是父进程写给子进程的读端，父进程不用读
         */
        close(child_to_father_arr[1]);
        close(father_to_child_arr[0]);

        int read_fd = child_to_father_arr[0];
        int write_fd = father_to_child_arr[1];

        char msg[] = "msg frommmmmmmmmm father";
        write(write_fd, msg, sizeof(msg));

        char buf[100] = {0};
        read(read_fd, buf, sizeof(buf));

        printf("father process read from pipe: %s\n", buf);

    }
    
    return 0;
}
