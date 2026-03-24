#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 现在想在一个文件中,启动两个进程--fork
    // 然后让这两个进程进行进程间通信--pipe

    mkfifo("1.pipe", 0600);
    pid_t ret = fork();

    if (ret == 0){
        // 子进程. 管道只开一端会阻塞
        printf("child process open before.\n");
        int fd = open("1.pipe", O_RDONLY);
        printf("child process open after.\n");

        char buf[100] = {0};
        read(fd, buf, sizeof(buf));
        printf("child process read buf: %s\n", buf);
        
        close(fd);
    }else{
        // 父进程
        printf("father process sleep before.\n");
        sleep(5);
        printf("father process sleep after.\n");
        int fd = open("1.pipe", O_WRONLY);
        printf("father process open after.\n");

        char content[] = "msg from father process";
        write(fd, content, sizeof(content));
        
        close(fd);
    }
    
    unlink("1.pipe");
    return 0;
}

