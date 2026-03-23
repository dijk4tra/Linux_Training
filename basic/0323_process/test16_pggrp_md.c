#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    /*
     * Linux/Unix 中，一个进程不仅有“进程 ID（pid）”，
     * 还有“进程组 ID（pgrp / pgid）”。
     *
     * 进程组可以理解为：把若干个相关进程归为一组，方便统一管理。
     * 例如 shell 启动一个前台任务时，这个任务里的多个进程通常会被放到同一个进程组里，
     * 这样 shell 就可以把信号（如 Ctrl+C 对应 SIGINT）发给整个进程组，而不是只发给单个进程。
     */

    /*
     * getpid()
     *   获取“当前进程”的进程 ID。
     *
     * getpgrp()
     *   获取“当前进程所属的进程组 ID”。
     *
     * 重点理解 getpgrp():
     *   1. 它返回的是“当前进程所在进程组”的 ID。
     *   2. 这个 ID 本质上通常等于“该进程组组长的 pid”。
     *   3. 它只能说明“该进程属于哪个组”。
     *   4. 一个进程的 pid 和 pgrp 可能相同，也可能不同：
     *        - 相同：当前进程刚好是该进程组的组长
     *        - 不同：当前进程只是组内普通成员
     *
     * 例如：
     *   假设某个进程组的组长 pid 是 5000，
     *   那么这个组里所有进程调用 getpgrp()，返回值都会是 5000。
     */

    printf("pid = %d, pgrp = %d\n", getpid(), getpgrp());
    
    /*
     * 程序刚运行时，当前进程通常继承父进程（一般是 shell）的进程组。
     * 所以这里的 pgrp 往往不是自己的 pid，而是 shell 所在前台任务的进程组 ID。
     *
     * 一般来说：
     *   - 普通程序启动后，一般先继承父进程的进程组；
     *   - 只有在显式调用 setpgid() / setsid()，或者由 shell 做作业控制时，
     *     才可能进入新的进程组。
     */

    if(fork() == 0){
        /*
         * setpgid(pid, pgid)
         *   用来设置某个进程所属的进程组。
         *
         * setpgid(0, 0) 的含义是：
         *   - 第一个 0：表示“当前进程”
         *   - 第二个 0：表示“把进程组 ID 设置成当前进程自己的 pid”
         *
         * 效果：
         *   让当前子进程创建一个新的进程组，并成为这个新进程组的组长。
         *
         * 调用成功后：
         *   子进程的 pgrp == 子进程的 pid
         */
        setpgid(0, 0);
        // 子进程：此时通常已经成为新进程组的组长
        printf("child process pid = %d, pgrp = %d\n", getpid(), getpgrp());
    }else{
        // 父进程没有修改自己的进程组，仍然保留原来的进程组ID
        printf("father process pid = %d, pgrp = %d\n", getpid(), getpgrp());
    }

    return 0;
}

