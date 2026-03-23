#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    /*
         * getpid()：获取“当前正在运行的这个进程”的进程 ID（PID）
         *
         * 重点理解：
         * - 每个进程都有自己唯一的 PID
         * - 在子进程里调用 getpid()，得到的是“子进程自己的 PID”
         * - 在父进程里调用 getpid()，得到的是“父进程自己的 PID”
         *
         * 例如：
         * 假设父进程 PID 是 1000，fork() 之后创建的子进程 PID 是 1001
         * 那么：
         * - 父进程中 getpid() 返回 1000
         * - 子进程中 getpid() 返回 1001
         */

        /*
         * getsid(0)：获取某个进程所属的会话 ID（Session ID）
         *
         * 参数说明：
         * - 传 0 表示“获取当前进程所属的会话 ID”
         *
         * 会话（session）可以简单理解成：
         * 一个登录终端下的一组相关进程的组织单位。
         *
         * 一般情况下：
         * - 你在终端中启动一个程序
         * - 这个程序及其 fork() 出来的子进程
         * - 默认都还在同一个会话中
         *
         * 所以本程序中，虽然父子进程 PID 不同，
         * 但通常它们的 SID（会话 ID）是相同的。
         *
         * 很多时候，这个 SID 会等于启动该会话的“会话首进程”的 PID。
         */

    if(fork() == 0){
        printf("child process, pid = %d, sid = %d\n", getpid(), getsid(0));
        sleep(10);
    }else{
        printf("father process, pid = %d, sid = %d\n", getpid(), getsid(0));
        sleep(10);
    }

    return 0;
}

