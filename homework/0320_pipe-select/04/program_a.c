#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    char file_name[100] = "test/1.txt";
    char file_content[4096] = "123456789abc";
    int name_len = strlen(file_name);
    int content_len = strlen(file_content);

    printf("程序A: 正在等待程序B打开管道\n");
    int fd = open("1.pipe", O_WRONLY);
    ERROR_CHECK(fd, -1, "open 1.pipe failed");

    // 发送文件名长度
    write(fd, &name_len, sizeof(int));
    // 发送文件名
    write(fd, file_name, name_len);
    // 发送文件内容长度
    write(fd, &content_len, sizeof(int));
    // 发送文件内容
    write(fd, file_content, content_len);

    close(fd);
    printf("程序A: 数据发送完毕, 已关闭管道\n");


    return 0;
}
