#include <my_header.h>

/*
完成窗口聊天：
通信方式：
    - A进程和B进程负责通信，从标准输入读到的字符串通过管道发给对方
    - A进程从标准输入读到的字符串发给B进程后，B打印到屏幕上。
    - B进程从标准输入读到的字符串发给A进程，A打印到屏幕上。
退出方式：
    任意一个进程在标准输入收到输入1，给所有进程（通信双方）发送10号信号，
    每个进程收到10号信号后，开始执行有序退出。
*/

#define FIFO_AB "ab.pipe"
#define FIFO_BA "ba.pipe"
#define PID_A "A.pid"
#define PID_B "B.pid"

volatile sig_atomic_t g_exit_flag = 0;

/* 信号处理函数：
 * 只做一件事：把退出标志置为1
 * 这是信号处理中最安全、最规范的写法
 */
void sig_func(int signo){
    g_exit_flag = 1;
}

/* 把自己的 pid 写入 pid 文件 */
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

/* 从 pid 文件中读取对方 pid */
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
    
    int fd_read = -1;
    int fd_write = -1;
    char buf[1024] = {0};

    /* 创建命名管道
     * 如果已经存在，则忽略 EEXIST 错误
     */
    if(mkfifo(FIFO_AB, 0666) == -1 && errno != EEXIST){
        perror("mkfifo ab.pipe failed");
        return -1;
    }
    if(mkfifo(FIFO_BA, 0666) == -1 && errno != EEXIST){
        perror("mkfifo ba.pipe failed");
        return -1;
    }

    /* 注册10号信号处理函数 */
    if(signal(10, sig_func) == SIG_ERR){
        perror("signal failed");
        return -1;
    }

    /* 将自己的pid写入自己的pid文件 */
    if(write_pid_file(PID_A) == -1){
        return -1;
    }
    
    /* A 的通信方向：
     * 读 ba.pipe（B -> A）
     * 写 ab.pipe（A -> B）
     *
     * 为了避免 open 时阻塞死锁：
     * 先开读端，再开写端
     * 两边程序启动后会成功配对
     */
    fd_read = open(FIFO_BA, O_RDONLY);
    if(fd_read == -1){
        perror("A open ba.pipe O_RDONLY failed");
        unlink(PID_A);
        return -1;
    }
    
    fd_write = open(FIFO_AB, O_WRONLY);
    if(fd_write == -1){
        perror("A open ab.pipe O_WRONLY failed");
        close(fd_read);
        unlink(PID_A);
        return -1;
    }

    printf("Process A start, pid = %d\n", getpid());
    printf("A: input message and press Enter\n");
    printf("A: input 1 to exit both processes\n");

    while(!g_exit_flag){
        fd_set rdset;
        FD_ZERO(&rdset);

        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fd_read, &rdset);

        int maxfd = (STDIN_FILENO > fd_read ? STDIN_FILENO : fd_read) + 1;
        int ret = select(maxfd, &rdset, NULL, NULL, NULL);
        if(ret == -1){
            if(errno == EINTR){
                /* 被信号打断，回到循环顶部检查退出标志 */
                continue;
            }
            perror("A select failed");
            break;
        }

        /* 1. 处理标准输入 */
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            
            /*
             * fgets 从标准输入读一行字符串
             * 会把末尾的 '\n' 一起读进来（如果这一行能完整读到）
             *
             * 返回 NULL 可能表示：
             * - 输入结束（例如 Ctrl + D）
             * - 读输入失败
             */
            if(fgets(buf, sizeof(buf), stdin) == NULL){
                /*
                 * 这里把 Ctrl + D 也当成退出请求处理：
                 * 给自己和对方都发送 10 号信号
                 */
                pid_t peer_pid = read_pid_file(PID_B);
                kill(getpid(), 10); // 给自己发 10 号信号
                if(peer_pid > 0){
                    kill(peer_pid, 10); // 给对方发 10 号信号
                }
                continue;
            }
            
            /*
             * fgets 读到的字符串通常会包含末尾换行 '\n'
             * 为了方便比较和发送，把这个换行去掉。
             *
             * strcspn(buf, "\n") 的作用：
             *   找到 buf 中第一次出现 '\n' 的位置
             * 如果没找到，就返回字符串长度。
             *
             * 把该位置改成 '\0'，就等于“截断掉换行”。
             */
            buf[strcspn(buf, "\n")] = '\0';
            /*
             * 如果输入的是 "1"
             * 则给通信双方都发送 10 号信号
             */
            if(strcmp(buf, "1") == 0){
                pid_t peer_pid = read_pid_file(PID_B);
                printf("A: input 1, send signal 10 to both processes.\n");
                kill(getpid(), 10); // 给自己发 10 号信号
                if(peer_pid > 0){
                    kill(peer_pid, 10); // 给对方发 10 号信号
                }
                continue;
            }

            /*
             * 普通消息处理：
             * 把字符串写到 ab.pipe 中，供 B 读取
             *
             * 为什么写 strlen(buf) + 1？
             * 因为希望把字符串结尾的 '\0' 也一起写过去，
             * 这样对端读到后可以直接按 C 字符串打印。
             */
            if(write(fd_write, buf, strlen(buf) + 1) == -1){
                perror("A write failed");
                break;
            }
        }

         /*
         * 情况2：FIFO 读端可读
         * 说明 B 发来了消息
         */
        if(FD_ISSET(fd_read, &rdset)){
            memset(buf, 0, sizeof(buf));
            
            // 从 ba.pipe 读取消息
            int read_count = read(fd_read, buf, sizeof(buf));

            if(read_count > 0){
                /*
                 * 成功读到数据
                 * 因为发送方把 '\0' 也写过来了，
                 * 所以这里可以直接当字符串打印
                 */
                printf("A recv from B: %s\n", buf);
            }else if(read_count == 0){
                /*
                 * read 返回 0 表示：
                 * 所有写端都关闭了，也就是对方不再写这个 FIFO 了
                 *
                 * 对于聊天程序来说，可以理解为：
                 *   对端已经退出/关闭写端
                 */
                printf("A: peer closed write end, prepare to exit.\n");
                break;
            }else{
                // n < 0 表示 read 出错
                if(errno == EINTR){
                    continue;
                }
                perror("A read failed");
                break;
            }
        }
    }

    /*
     * 跳出主循环后，开始有序退出.
     * 所谓“有序退出”，就是把程序运行过程中申请/打开的资源按顺序释放。
     */
    close(fd_read);
    close(fd_write);
    
    /*
     * 删除自己的 pid 文件
     *
     * unlink 的作用是删除目录项。
     * 对于 pid 文件来说，就是把 A.pid 删除掉，
     * 避免下次运行时残留旧 pid。
     */
    unlink(PID_A);
    
    printf("A orderly exit.\n");

    return 0;
}

