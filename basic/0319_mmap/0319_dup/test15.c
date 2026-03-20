#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    // 因为在第一次使用输出的时候，行为和终端行为保持一致
    // 如果第一次输出，指向的是终端，就是一个行缓冲区
    // 如果第一次输出，指向的是文件，就是满缓冲区
    printf("-------------");  // 这行代码的有无会影响后续代码的输出

    ARGS_CHECK(argc, 2);
    int fd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0664);
    ERROR_CHECK(fd,-1,"open");

    //先备份标准输出的文件描述符
    int temp=dup(STDOUT_FILENO);
    //将标准输出重定向指向文件
    dup2(fd, STDOUT_FILENO);
    printf("How are you?\n");
    //将标准输出重新终端
    dup2(temp, STDOUT_FILENO);
    printf("I'm fine, and you?\n");
    //再将标准输出重定向到文件
    dup2(fd, STDOUT_FILENO);
    printf("Me too.\n");

    close(fd);
    
    return 0;
}

