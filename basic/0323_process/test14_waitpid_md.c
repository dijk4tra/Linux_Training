#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // waitpid默认是一个阻塞函数, 它会等待指定的一个子进程, 直到这个子进程结束
    // 我们可以将其入参options设置为 WNOHANG, 这样会让waitpid不再阻塞

    pid_t fork_ret = fork();
    /*
     * fork()：创建子进程
     * 返回值说明：
     *   = 0  : 当前代码运行在子进程中
     *   > 0  : 当前代码运行在父进程中，返回值是子进程的 pid
     *   < 0  : 创建失败
     */

    if(fork_ret == 0){
        // 子进程
        printf("child process begin.\n");
        sleep(30);
        printf("child process over.\n");
    }else{
        printf("father process begin.\n");

        /*
         * waitpid()：等待“指定的子进程”结束
         *
         * 函数原型：
         *   pid_t waitpid(pid_t pid, int *wstatus, int options);
         *
         * 参数说明：
         * 1. pid
         *    - > 0  : 等待指定 pid 的子进程
         *             这里传 fork_ret，表示只等待刚刚 fork 出来的这个子进程
         *    - = -1 : 等待任意一个子进程，作用类似 wait()
         *    - = 0  : 等待与当前进程同一进程组中的任意子进程
         *    - < -1 : 等待指定进程组中的任意子进程
         *
         * 2. wstatus
         *    - 用来保存子进程结束状态
         *    - 这里传 NULL，表示我们暂时不关心子进程是正常退出还是异常退出
         *
         * 3. options
         *    - 0       : 默认阻塞等待
         *                如果子进程还没结束，父进程会卡在这里，一直等到子进程退出
         *    - WNOHANG : 非阻塞等待
         *                如果子进程还没结束，waitpid() 会立即返回 0，不会阻塞父进程
         *
         * 当前这句代码：
         *   waitpid(fork_ret, NULL, 0);
         * 因为 options = 0，所以是“阻塞等待”。
         * 也就是说，父进程会一直停在这里，直到子进程 sleep(20) 结束并退出。
         *
         * 返回值说明：
         *   > 0 : 返回已经结束的子进程 pid
         *    0  : 只有设置了 WNOHANG，且子进程还没退出时才会返回 0
         *   -1  : 调用失败
         *
         * 为什么要用 waitpid() / wait()？
         * - 父进程如果不回收子进程资源，子进程结束后会变成僵尸进程
         * - waitpid() 可以精确等待某一个子进程，比 wait() 更灵活
         */

        /*
         * 如果改成：
         *     pid_t waitpid_ret = waitpid(fork_ret, NULL, WNOHANG);
         *  
         * 那么 waitpid() 就变成“非阻塞等待”。
         *
         * 情况1：子进程还没有结束
         *     返回 0
         *     父进程不会卡住，可以继续往下执行别的代码
         *
         * 情况2：子进程已经结束
         *     返回子进程的 pid
         *
         * 情况3：调用失败
         *     返回 -1
         *
         * 这种方式常用于：
         * - 父进程不想一直等
         * - 父进程还要继续处理别的任务
         * - 周期性检查子进程是否退出
         */

        pid_t wait_pid_ret = waitpid(fork_ret, NULL, WNOHANG);

        printf("father process waitpid over. ret = %d\n", wait_pid_ret);
    }

    return 0;
}

