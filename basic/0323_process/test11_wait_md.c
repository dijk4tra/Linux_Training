#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // fork的返回值, 用于确认目前在父进程中, 还是在子进程中
    pid_t fork_ret = fork();

    if(fork_ret == 0){
        // 这是子进程
        printf("child process.\n");
        sleep(30);
        printf("child process over.\n");
    }else{
        printf("father process, wait before.\n");
        sleep(3);
        printf("father process, sleep after(wait before).\n");
        
        /*
         * wait(NULL) 的核心作用：
         * -----------------------------------------
         * 父进程在这里“等待任意一个子进程结束”。
         *
         * 1. wait() 是阻塞函数
         *    如果子进程还没结束，父进程会停在 wait() 这里不继续向下执行。
         *
         * 2. 当某个子进程结束后：
         *    - wait() 会立即返回
         *    - 返回值是“已经结束的那个子进程的 PID”
         *
         * 3. wait() 还能顺便回收子进程的退出资源
         *    子进程结束后，并不会马上从系统里彻底消失，
         *    它会暂时变成“僵尸进程”，等待父进程读取它的退出状态。
         *    父进程如果不调用 wait()/waitpid() 回收，
         *    这个子进程就会一直保持僵尸状态。
         *
         * 4. 为什么这里传 NULL？
         *    wait() 原型大致是：
         *        pid_t wait(int *status);
         *
         *    - 如果传入非 NULL，父进程可以通过 status 获取子进程退出状态
         *    - 这里传 NULL，表示“我只关心等它结束并回收它，
         *      不关心它是怎么退出的”
         *
         * 结合本程序的执行过程：
         * - 子进程要 sleep(30)
         * - 父进程只 sleep(3)
         * - 所以父进程运行到 wait(NULL) 时，子进程大概率还没结束
         * - 因此父进程会在 wait(NULL) 这里阻塞大约 27 秒左右
         * - 等子进程打印 "child process over." 并退出后，
         *   wait() 才返回，父进程继续向下执行
         */
        pid_t wait_ret = wait(NULL);
        /*
         * wait() 返回值说明：
         * - 成功：返回被回收的子进程 PID
         * - 失败：返回 -1
         *
         * 在这个程序中，只有一个子进程，
         * 所以 wait_ret 一般就是 fork_ret。
         */
        printf("father process, wait after, wait_ret = %d.\n", wait_ret);

        // wait() = “父进程暂停，直到某个子进程结束，并顺便把它回收掉”
    }
     
    return 0;
}

