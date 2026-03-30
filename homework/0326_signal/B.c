#include <my_header.h>

/*
 * 两个命名管道：
 * ab.pipe: A -> B
 * ba.pipe: B -> A
 *
 * B 进程的职责：
 * - 从标准输入读取自己输入的消息
 * - 把消息写入 ba.pipe，让 A 读到
 * - 同时从 ab.pipe 中读取 A 发来的消息并打印
 */
#define FIFO_AB "ab.pipe"
#define FIFO_BA "ba.pipe"

/*
 * pid 文件：
 * - A.pid 存放 A 的 pid
 * - B.pid 存放 B 的 pid
 *
 * B 想给 A 发送 10 号信号时，
 * 需要先读取 A.pid 获得 A 的进程号。
 */
#define PID_A   "A.pid"
#define PID_B   "B.pid"

/*
 * 全局退出标志
 * 收到 10 号信号后，在信号处理函数中把它置为 1，
 * 主循环检测到后执行有序退出。
 */
volatile sig_atomic_t g_exit_flag = 0;

/*
 * 信号处理函数
 * 只置退出标志，不做复杂逻辑
 */
void sig_func(int signo){
    g_exit_flag = 1;
}

/*
 * 把当前进程 pid 写入指定 pid 文件
 * B 会把自己的 pid 写入 B.pid
 */
int write_pid_file(const char *filename){
    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        perror("fopen pid file failed");
        return -1;
    }
    fprintf(fp, "%d\n", getpid());
    fclose(fp);
    return 0;
}

/*
 * 从指定 pid 文件读取 pid
 * B 读取 A.pid 时，就能知道 A 的 pid
 */
int read_pid_file(const char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return -1;
    }
    pid_t pid;
    if(fscanf(fp, "%d", &pid) != 1){
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return pid;
}


int main(int argc, char *argv[]){                                  
    /*
     * fd_read:
     *   B 的读端，读取 A 发来的消息
     *   所以 B 读 ab.pipe
     *
     * fd_write:
     *   B 的写端，给 A 发送消息
     *   所以 B 写 ba.pipe
     */ 
    int fd_read = -1;
    int fd_write = -1;
    char buf[1024] = {0};

    /*
     * 创建 ab.pipe
     * 若已存在则忽略 EEXIST
     */
    if(mkfifo(FIFO_AB, 0666) == -1 && errno != EEXIST){
        perror("mkfifo ab.pipe failed");
        return -1;
    }
    if(mkfifo(FIFO_BA, 0666) == -1 && errno != EEXIST){
        perror("mkfifo ba.pipe failed");
        return -1;
    }

    // 注册 10 号信号处理函数
    if(signal(10, sig_func) == SIG_ERR){
        perror("signal failed");
        return -1;
    }

    /*
     * B 把自己的 pid 写入 B.pid
     * 这样 A 就能通过读取 B.pid 找到 B
     */
    if(write_pid_file(PID_B) == -1){
        return -1;
    }

     /*
     * B 的通信方向：
     *   读 ab.pipe（A -> B）
     *   写 ba.pipe（B -> A）
     */
    fd_write = open(FIFO_BA, O_WRONLY);
    if(fd_write == -1){
        perror("B open ba.pipe O_WRONLY failed");
        unlink(PID_B);
        return -1;
    }
    fd_read = open(FIFO_AB, O_RDONLY);
    if(fd_read == -1){
        perror("B open ab.pipe O_RDONLY failed");
        close(fd_write);
        unlink(PID_B);
        return -1;
    }
    
    printf("Process B start, pid = %d\n", getpid());
    printf("B: input message and press Enter\n");
    printf("B: input 1 to exit both processes\n");

    /*
     * 主循环：
     * 使用 select 同时监听：
     * 1. 标准输入
     * 2. 来自 A 的 FIFO 输入
     */
    while(!g_exit_flag){
        fd_set rdset;
        FD_ZERO(&rdset);
        
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fd_read, &rdset);

        int maxfd = (STDIN_FILENO > fd_read ? STDIN_FILENO : fd_read) + 1;

        int ret = select(maxfd, &rdset, NULL, NULL, NULL);
        if(ret == -1){
            if(errno == EINTR){
                continue;
            }
            perror("B select failed");
            break;
        }
        
        /*
         * 标准输入可读：
         * 表示当前终端中，用户给 B 输入了内容
         */
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));

            if(fgets(buf, sizeof(buf), stdin) == NULL){
                /*
                 * 例如 Ctrl + D
                 * 这里也按“双方退出”处理
                 */
                pid_t peer_pid = read_pid_file(PID_A);

                kill(getpid(), 10);
                if(peer_pid > 0){
                    kill(peer_pid, 10);
                }
                continue;
            }

            /* 去掉末尾换行 */
            buf[strcspn(buf, "\n")] = '\0';

            /*
             * 输入 1：
             * 给自己和 A 都发送 10 号信号
             */
            if(strcmp(buf, "1") == 0){
                pid_t peer_pid = read_pid_file(PID_A);

                printf("B: input 1, send signal 10 to both processes.\n");

                kill(getpid(), 10);
                if(peer_pid > 0){
                    kill(peer_pid, 10);
                }else{
                    printf("B: cannot read A pid, only self-exit signal sent.\n");
                }
                continue;
            }

            /*
             * 普通消息：
             * B 把消息写入 ba.pipe，让 A 读取
             */
            if(write(fd_write, buf, strlen(buf)+1) == -1){
                perror("B write failed");
                break;
            }
        }

        /*
         * FIFO 可读：
         * 说明 A 发来了消息
         */
        if(FD_ISSET(fd_read, &rdset)){
            memset(buf, 0, sizeof(buf));
            int read_count = read(fd_read, buf, sizeof(buf));

            if(read_count > 0){
                printf("B recv from A: %s\n", buf);
            }else if(read_count == 0){
                // 对端关闭了写端
                printf("B: peer closed write end, prepare to exit.\n");
                break;
            }else{
                if(errno == EINTR){
                    continue;
                }
                perror("B read");
                break;
            }
        }
    }

    /*
     * 有序退出：关闭文件描述符，删除 pid 文件
     */
    close(fd_read);
    close(fd_write);

    unlink(PID_B);

    printf("B orderly exit.\n");

    return 0;
}

