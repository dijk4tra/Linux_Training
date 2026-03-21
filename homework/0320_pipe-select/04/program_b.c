#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("程序B: 正在打开管道, 准备读取数据...\n");
    int fd = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd, -1, "open 1.pipe failed");

    int name_len = 0;
    read(fd, &name_len, sizeof(int)); // 读文件名长度

    char file_path[128] = {0};
    read(fd, file_path, name_len); // 读文件名
    file_path[name_len] = '\0'; // 确保字符串以 \0 结尾
    printf("程序B: 接收到文件名 %s\n", file_path);

    // 如果目录不存在, 则创建目录
    if (access("test", F_OK) == -1){
        int ret = mkdir("test", 0777);
        ERROR_CHECK(ret, -1, "mkdir failed");
        printf("程序B: 目录test不存在, 已新建.\n");
    }
    
    int content_len = 0;
    read(fd, &content_len, sizeof(int)); // 读文件内容长度
    printf("程序B: 接收到文件内容长度: %d\n", content_len);

    char buffer[4096] = {0};
    read(fd, buffer, content_len); // 读文件内容
    printf("程序B: 接收到文件内容");

    int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    system("pwd");
    ERROR_CHECK(file_fd, -1, "open file failed");

    write(file_fd, buffer, content_len);
    
    printf("程序B: 已将内容写入文件 %s\n", file_path);

    close(fd);
    close(file_fd);

    return 0;
}

