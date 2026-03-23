#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    // waitpid默认是一个阻塞函数,它会等待指定的一个子进程,等待其结束
    // 我们可以可以给options设置一个值. WNOHANG会使waitpid不再阻塞

    pid_t fork_ret = fork();

    if(fork_ret == 0){
        // 子进程
        printf("child process begin.\n");
        sleep(30);
        printf("child process over.\n");
    }else{
        printf("father process begin.\n");
        // -----------时间戳1-------------
        // 用来记录是否成功等到子进程退出
        bool wait_success = false;

        pid_t wait_pid_ret = 0;
        
        /*
         * 这里循环 60 次，每秒检查一次子进程是否结束。
         *
         * 整体效果：
         *   - 子进程总共睡 30 秒
         *   - 父进程每隔 1 秒调用一次 waitpid(..., WNOHANG)
         *   - 在前 30 秒左右，waitpid() 大概率返回 0，表示“子进程还活着”
         *   - 子进程结束后，某一次 waitpid() 会返回子进程 pid，表示成功回收
         *
         * 这是“轮询 + 非阻塞等待”的典型写法。
         */

        for(int i=0; i<60; i++){
            pid_t wait_pid_ret = waitpid(fork_ret, NULL, WNOHANG);
            if(wait_pid_ret > 0){
                // 返回值>0,说明等到了目标子进程
                wait_success = true;
                break;
            }else if (wait_pid_ret == 0) {
                /*
                 * 返回 0：
                 *   只有在 options 设置为 WNOHANG 时才会这样
                 *   表示“子进程还没有退出”
                 *   此时父进程不会阻塞，可以继续执行别的逻辑
                 */
                printf("child is still running...\n");
            }else{
                /*
                 * 返回 -1：
                 *   表示 waitpid() 调用失败
                 *   常见原因：
                 *   1. 传入的 pid 不合法
                 *   2. 该 pid 不是当前进程的子进程
                 *   3. 子进程已经被回收过了
                 */
                perror("waitpid failed");
                break;
            }
            
            // 循环60轮
            // 睡1秒，避免疯狂轮询占用CPU
            sleep(1);
        }
        
        /*
         * wait_success == true：
         *   说明在 60 次轮询内，成功等到子进程结束并完成回收
         *
         * wait_success == false：
         *   说明在限定时间内没有等到，或者 waitpid() 中途出错
         *   这时可以选择继续重试、记录日志、执行其他处理逻辑等
         */

        if(wait_success){
            // 进行后续操作
            printf("wait success, child has exited.\n");
        }else{
            // retry()
            printf("wait timeout or error, you can retry later.\n");
        }

        // --------------时间戳2-----------------
        printf("father process waitpid over. ret = %d\n", wait_pid_ret);
    }

    return 0;
}

